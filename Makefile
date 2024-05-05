# This Makefile is just a script runner, all scripts are in the scripts folder
run:
	@make build
	@./bin/release/app

dev:
	@make build-d
	@./bin/debug/app

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

.PHONY: run dev build build-d test clear