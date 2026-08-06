
.PHONY: check-cran check-cran-extra check-% clean install docs

# CRAN-like containers (pair: CRAN name : r-hub image)
CRAN_PAIRS := \
	r-devel-linux-x86_64-debian-clang:ubuntu-clang \
 	r-devel-linux-x86_64-debian-gcc:ubuntu-gcc16 \
 	r-patched-linux-x86_64:ubuntu-next \
 	r-release-linux-x86_64:ubuntu-release

# Extra CRAN check images
CRAN_EXTRA := atlas clang-asan clang-ubsan clang21 clang22 donttest \
	gcc16 gcc-asan lto mkl nold nosuggests rchk valgrind

check-cran:
	@chmod +x ./check-docker/check.sh
	@for pair in $(CRAN_PAIRS); do \
		cran=$${pair%%:*}; rhub=$${pair##*:}; \
		echo "=== checking $$cran (r-hub: $$rhub) ==="; \
		./check-docker/check.sh $$rhub; \
	done

check-cran-extra:
	@chmod +x ./check-docker/check.sh
	@for rhub in $(CRAN_EXTRA); do \
		echo "=== checking $$rhub ==="; \
		./check-docker/check.sh $$rhub; \
	done

# ubuntu-gcc16 with GCC 16 (local build until r-hub accepts the PR)
check-ubuntu-gcc16:
	@chmod +x ./check-docker/check.sh
	@docker buildx build -t redatam-ubuntu-gcc16 -f ./check-docker/Dockerfile-ubuntu-gcc16 .
	@FULL_IMAGE_OVERRIDE=redatam-ubuntu-gcc16:latest ./check-docker/check.sh ubuntu-gcc16

# Individual check target, e.g. `make check-clang22`
check-%:
	@chmod +x ./check-docker/check.sh
	@./check-docker/check.sh $*

clean:
	@Rscript -e 'tinydev::pkg_clean(".");'

install:
	@Rscript -e 'tinydev::pkg_install(".")'

clang_format=`which clang-format-21`

format: $(shell find . -not -path './check-docker/*' -not -path './src/vendor/*' -name '*.h') $(shell find . -not -path './check-docker/*' -not -path './src/vendor/*' -name '*.hpp') $(shell find . -not -path './check-docker/*' -not -path './src/vendor/*' -name '*.cpp')
	@${clang_format} -i $?
