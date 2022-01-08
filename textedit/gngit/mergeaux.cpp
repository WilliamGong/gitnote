#include "repo.h"

/**
 * These functions are all from libgit2/examples. 
 * Only modified sightly to run with gngit. 
 * 
 */

static int resolve_heads_s(git_repository *repo, merge_options *opts) {
	git_annotated_commit **annotated = new git_annotated_commit* [opts->heads_count];
	size_t annotated_count = 0, i;
	int err = 0;

	for (i = 0; i < opts->heads_count; i++) {
		err = resolve_refish(&annotated[annotated_count++], repo, opts->heads[i]);
		if (err != 0) {
			fprintf(stderr, "failed to resolve refish %s: %s\n", opts->heads[i], git_error_last()->message);
			annotated_count--;
			continue;
		}
	}

	if (annotated_count != opts->heads_count) {
		fprintf(stderr, "unable to parse some refish\n");
        delete[] annotated;
		return -1;
	}

	opts->annotated = annotated;
	opts->annotated_count = annotated_count;
	return 0;
}

int resolve_heads(git_repository *repo, merge_options *opts) {
    return resolve_heads_s(repo, opts);
}

int resolve_refish(git_annotated_commit **commit, git_repository *repo, const char *refish) {
	git_reference *ref;
	git_object *obj;
	int err = 0;

	assert(commit != NULL);

	err = git_reference_dwim(&ref, repo, refish);
	if (err == GIT_OK) {
		git_annotated_commit_from_ref(commit, repo, ref);
		git_reference_free(ref);
		return 0;
	}

	err = git_revparse_single(&obj, repo, refish);
	if (err == GIT_OK) {
		err = git_annotated_commit_lookup(commit, repo, git_object_id(obj));
		git_object_free(obj);
	}

	return err;
}

static int perform_fastforward_s(git_repository *repo, const git_oid *target_oid, int is_unborn) {
	git_checkout_options ff_checkout_options = GIT_CHECKOUT_OPTIONS_INIT;
	git_reference *target_ref;
	git_reference *new_target_ref;
	git_object *target = NULL;
	int err = 0;

	if (is_unborn) {
		const char *symbolic_ref;
		git_reference *head_ref;

		/* HEAD reference is unborn, lookup manually so we don't try to resolve it */
		err = git_reference_lookup(&head_ref, repo, "HEAD");
		if (err != 0) {
			fprintf(stderr, "failed to lookup HEAD ref\n");
			return -1;
		}

		/* Grab the reference HEAD should be pointing to */
		symbolic_ref = git_reference_symbolic_target(head_ref);

		/* Create our master reference on the target OID */
		err = git_reference_create(&target_ref, repo, symbolic_ref, target_oid, 0, NULL);
		if (err != 0) {
			fprintf(stderr, "failed to create master reference\n");
			return -1;
		}

		git_reference_free(head_ref);
	} else {
		/* HEAD exists, just lookup and resolve */
		err = git_repository_head(&target_ref, repo);
		if (err != 0) {
			fprintf(stderr, "failed to get HEAD reference\n");
			return -1;
		}
	}

	/* Lookup the target object */
	err = git_object_lookup(&target, repo, target_oid, GIT_OBJECT_COMMIT);
	if (err != 0) {
		fprintf(stderr, "failed to lookup OID %s\n", git_oid_tostr_s(target_oid));
		return -1;
	}

	/* Checkout the result so the workdir is in the expected state */
	ff_checkout_options.checkout_strategy = GIT_CHECKOUT_SAFE;
	err = git_checkout_tree(repo, target, &ff_checkout_options);
	if (err != 0) {
		fprintf(stderr, "failed to checkout HEAD reference\n");
		return -1;
	}

	/* Move the target reference to the target OID */
	err = git_reference_set_target(&new_target_ref, target_ref, target_oid, NULL);
	if (err != 0) {
		fprintf(stderr, "failed to move HEAD reference\n");
		return -1;
	}

	git_reference_free(target_ref);
	git_reference_free(new_target_ref);
	git_object_free(target);

	return 0;
}

int perform_fastforward(git_repository *repo, const git_oid *target_oid, int is_unborn) {
    return perform_fastforward_s(repo, target_oid, is_unborn);
}