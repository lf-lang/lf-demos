.PHONY: help format clean

# Default target - show help message
help:
	@echo "Lingua Franca Demos - Available Make Targets:"
	@echo ""
	@echo "  make help     - Display this help message (default)"
	@echo "  make format   - Format all .lf files in the repository using lff"
	@echo "  make clean    - Remove build artifacts (build, include, bin, src-gen, fed-gen)"
	@echo "                  from all directories containing a 'src' subdirectory"
	@echo ""

# Format all .lf files in the repository using lff
format:
	@echo "Formatting all .lf files..."
	@find . -name "*.lf" -type f | while read file; do \
		echo "  ===============Formatting $$file"; \
		lff "$$file"; \
	done
	@echo "Formatting complete."

# Clean build artifacts from directories containing a src subdirectory
clean:
	@echo "Cleaning build artifacts..."
	@find . -type d -name "src" | while read srcdir; do \
		parentdir=$$(dirname "$$srcdir"); \
		if [ "$$parentdir" != "." ]; then \
			echo "  Cleaning directory: $$parentdir"; \
			for dir in build include bin src-gen fed-gen; do \
				if [ -d "$$parentdir/$$dir" ]; then \
					echo "    Removing $$parentdir/$$dir"; \
					rm -rf "$$parentdir/$$dir"; \
				fi \
			done \
		fi \
	done
	@echo "Cleaning complete."

