SHELL = /bin/sh

## Copyright 2015-2016 Oliver Heimlich
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
VERSION = $(shell grep "^/// \\\\version " ../C++/src/vibes.h | $(CUT) -f3 -d" " | sed -e "s/[^0-9.]//g")
DATE = $(shell date "+%Y-%m-%d")

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
	@# The C++ API must be bundled
	@rm -rf "$(PACKAGE)-bundle"
	@mkdir -p "$(PACKAGE)-bundle"
	@git archive --format=tar --prefix="$(PACKAGE)-$(VERSION)/" HEAD | \
		(cd "$(PACKAGE)-bundle" && tar xf -)
	@(cd ../C++/; git archive --format=tar --prefix="$(PACKAGE)-$(VERSION)/" HEAD src) | \
		(cd "$(PACKAGE)-bundle" && tar xf -)
	@sed -i -e "s/%VERSION%/$(VERSION)/" "$(PACKAGE)-bundle/$(PACKAGE)-$(VERSION)/DESCRIPTION"
	@sed -i -e "s/%DATE%/$(DATE)/" "$(PACKAGE)-bundle/$(PACKAGE)-$(VERSION)/DESCRIPTION"
	@(cd "$(PACKAGE)-bundle" && tar czf "../$@" "$(PACKAGE)-$(VERSION)")

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
FORGE_HTML_TARBALL = $(HTML_DIR)-forge.tar.gz

.PHONY: forge-html html release md5
forge-html: $(FORGE_HTML_TARBALL)
html: $(HTML_TARBALL)
release: $(RELEASE_TARBALL) $(FORGE_HTML_TARBALL) $(HTML_TARBALL) md5

$(HTML_TARBALL): $(INSTALLED_PACKAGE)
	@echo "Generating HTML documentation for the package (GitHub) ..."
	@$(RM) -r "$(HTML_DIR)"
	@# Make sure that the generate_html package is present
	@$(OCTAVE) --no-gui --silent \
		--eval "try;" \
		--eval "temp = pkg ('describe', 'generate_html');" \
		--eval "catch;" \
		--eval "pkg ('install', '-forge', 'generate_html');" \
		--eval "end_try_catch;"
	@$(OCTAVE) --no-gui --silent \
		--eval "pkg load generate_html;" \
		--eval "options = get_html_options ('octave');" \
		--eval "options.overview_title = 'VIBes Client API for Octave';" \
		--eval "options.css = '/sylesheets/style.css';" \
		--eval "options.header = '<!DOCTYPE html><html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=%charset\"/><title>VIBes Client API for Octave - %title</title><link rel=\"stylesheet\" href=\"%css\"/><style type=\"text/css\">h3.category {margin-top: 2em}</style></head><body><h1>%title</h1>';" \
		--eval "options.footer = '</body></html>';" \
		--eval "generate_package_html ('$(PACKAGE)', '$(HTML_DIR)', options)"
	@mv "$(HTML_DIR)/$(PACKAGE)" "$(HTML_DIR)/octave-api"
	@tar --create --auto-compress --file "$@" "$(HTML_DIR)"

$(FORGE_HTML_TARBALL): $(INSTALLED_PACKAGE)
	@echo "Generating HTML documentation for the package (Octave Forge) ..."
	@$(RM) -r "$(HTML_DIR)"
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

md5: $(RELEASE_TARBALL) $(HTML_TARBALL) $(FORGE_HTML_TARBALL)
	@md5sum $^

##
## Testing Process
##
## Interactive shell with the package installed and loaded /
## Non-interactive processing of integrated unit tests
##

.PHONY: run check
PUBLIC_FUNCTIONS = $(patsubst inst/%.m,%,$(wildcard inst/+vibes/*.m))

run: $(INSTALLED_PACKAGE)
	@echo "Run GNU Octave with the development version of the package"
	@$(OCTAVE) --no-gui --silent --eval "pkg load $(PACKAGE)" --persist
	@echo

check: $(INSTALLED_PACKAGE)
	@echo "Testing package in GNU Octave ..."
	@# Make sure that the interval package is present, some tests depend on this
	@$(OCTAVE) --no-gui --silent \
		--eval "try;" \
		--eval "temp = pkg ('describe', 'interval');" \
		--eval "catch;" \
		--eval "pkg ('install', '-forge', 'interval');" \
		--eval "end_try_catch;"
	@rm -f ~/.vibes.json
	@$(OCTAVE) --no-gui --silent \
		--eval "pkg load $(PACKAGE);" \
		--eval "success = true;" \
		--eval "targets = strsplit ('$(PUBLIC_FUNCTIONS)', ' ');" \
		--eval "for target = targets;" \
		--eval "[n, nmax] = test (target{1}, 'verbose');" \
		--eval "success &= (n == nmax);" \
		--eval "endfor;" \
		--eval "exit (not (success));"
	@cat ~/.vibes.json

##
## Cleanup
##

.PHONY: clean

clean:
	rm -rf \
		$(PACKAGE)-*.tar.gz \
		"$(PACKAGE)-bundle" \
		"$(HTML_DIR)" \
		"$(HTML_TARBALL)" \
		"fntests.log" \
		octave-*

