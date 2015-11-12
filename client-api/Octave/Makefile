SHELL = /bin/sh

## Copyright 2015 Oliver Heimlich
##
## This file is part of VIBes' API for Octave.
##
## VIBes' API for Octave is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## VIBes' API for Octave is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with VIBes' API for Octave.
## If not, see <http://www.gnu.org/licenses/>.

CUT ?= cut
PACKAGE = $(shell grep "^Name: " DESCRIPTION | $(CUT) -f2 -d" ")
VERSION = $(shell grep "^Version: " DESCRIPTION | $(CUT) -f2 -d" ")
DATE = $(shell grep "^Date: " DESCRIPTION | $(CUT) -f2 -d" ")

##
## Release Process
##
## Create a tarball distribution for the Octave package
##

GZIP ?= gzip
RELEASE_TARBALL = $(PACKAGE)-$(VERSION).tar.gz

.PHONY: dist
dist: $(RELEASE_TARBALL)

$(RELEASE_TARBALL): ../../.git/index
	@echo "Creating package release ..."
	@# Export the current subdir of the source repository
	@# This is only the Octave API
	@git archive --output="$(PACKAGE)-$(VERSION).tar" --prefix="$(PACKAGE)-$(VERSION)/" HEAD
	@# The C++ API must be bundled
	@(cd ../C++/; git archive --output="$(shell pwd)/$(PACKAGE)-deps.tar" --prefix="$(PACKAGE)-$(VERSION)/" HEAD src)
	@tar --concatenate --file="$(PACKAGE)-$(VERSION).tar" "$(PACKAGE)-deps.tar"
	@$(GZIP) --best -f "$(PACKAGE)-$(VERSION).tar"
	@rm -f "$(PACKAGE)-$(VERSION).tar" "$(PACKAGE)-deps.tar"

##
## Local Install Process
##
## Do a local installation for testing in Octave, this will compile the package
##

OCTAVE ?= octave
MKOCTFILE ?= mkoctfile -Wall
INSTALLED_PACKAGE_DIR = ~/octave/$(PACKAGE)-$(VERSION)
INSTALLED_PACKAGE = $(INSTALLED_PACKAGE_DIR)/packinfo/DESCRIPTION

.PHONY: install
install: $(INSTALLED_PACKAGE)

$(INSTALLED_PACKAGE): $(RELEASE_TARBALL)
	@echo "Installing package in GNU Octave ..."
	@$(OCTAVE) --no-gui --silent --eval "pkg install $<"

##
## Publishing Process
##
## Also generate package documentation for the release
##

HTML_DIR = $(PACKAGE)-html
HTML_TARBALL = $(HTML_DIR).tar.gz

.PHONY: html release md5
html: $(HTML_TARBALL)
release: $(RELEASE_TARBALL) $(HTML_TARBALL) md5

$(HTML_TARBALL): $(INSTALLED_PACKAGE)
	@echo "Generating HTML documentation for the package ..."
	@# Make sure that the generate_html package is present
	@$(OCTAVE) --no-gui --silent \
		--eval "try;" \
		--eval "temp = pkg ('describe', 'generate_html');" \
		--eval "catch;" \
		--eval "pkg ('install', '-forge', 'generate_html');" \
		--eval "end_try_catch;"
	@$(OCTAVE) --no-gui --silent \
		--eval "pkg load generate_html;" \
		--eval "options = get_html_options ('octave-forge');" \
		--eval "generate_package_html ('$(PACKAGE)', '$(HTML_DIR)', options)"
	@tar --create --auto-compress --file "$@" "$(HTML_DIR)"

md5:  $(RELEASE_TARBALL) $(HTML_TARBALL)
	@md5sum $^

##
## Testing Process
##
## Interactive shell with the package installed and loaded
##

.PHONY: run

run: $(INSTALLED_PACKAGE)
	@echo "Run GNU Octave with the development version of the package"
	@$(OCTAVE) --no-gui --silent --eval "pkg load $(PACKAGE)" --persist
	@echo

##
## Cleanup
##

.PHONY: clean

clean:
	rm -f \
		"$(PACKAGE)-*.tar" \
		"$(PACKAGE)-*.tar.gz" \
		"$(HTML_DIR)" \
		"$(HTML_TARBALL)" \
		"fntests.log" \
		"octave-*"

