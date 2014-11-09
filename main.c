/*
 * Copyright (c) 2014 Andreas Fett.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * gitwrap: prevent forced push to and deletion of remote master branch.
 *
 * Rename the binary to 'git' and place it in a directory which is located
 * in $PATH before the directory where the real git lives.
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool match(const char *arg, const char *word)
{
	return strcmp(arg, word) == 0;
}

int main(int argc, char *argv[])
{
	enum {
		START,
		COMMAND,
		PUSH,
		FORCED,
	} state = START;

	for (int i = 1; i < argc && state != COMMAND; ++i) {
		switch (state) {
		case START:
			if (argv[i][0] != '-') {
				state = (match(argv[i], "push") ? PUSH : COMMAND);
			}
			break;
		case COMMAND:
			assert(false);
			break;
		case PUSH:
			if (match(argv[i], "-f") || match(argv[i], "--force")) {
				state = FORCED;
			} else if (match(argv[i], ":master")) {
				fprintf(stderr, "aborting delete of master\n");
				return EXIT_FAILURE;
			}
			break;
		case FORCED:
			if (match(argv[i], "master")) {
				fprintf(stderr, "aborting forced push on master\n");
				return EXIT_FAILURE;
			}
			break;
		}
	}

	return execv(GIT_BINARY_PATH, argv) != -1 ?
		EXIT_SUCCESS : EXIT_FAILURE;
}
