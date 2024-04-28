# This Makefile is just a script runner, all scripts are in the scripts folder

build:
	@./scripts/build.sh release

build-d:
	@./scripts/build.sh debug

test:
	@./scripts/test.sh

clear:
	rm -rf ./build
	rm -rf ./bin
	@echo "clear complete"

.PHONY: build build-d test clear