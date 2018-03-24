all: create-single-header

.PHONY: create-single-header
create-single-header: single_include-prep single_include-contents single_include-post single_include-format

.PHONY: single_include-prep
single_include-prep:
	$(shell cat preamble.inc > gethin.hpp)

.PHONY: single_include-contents
single_include-contents: single_include-prep
	# sed, get all include filenames from the common include
	# xargs echo, add a newline at the on dof the sed output
	# dos2unix, handle crlf
	# for each include-file
	#	cat the file but remove namespaces and preprocessor directives
	# push everything to the common header file
	$(shell sed -n 's/#include "\(.*\)"/\1/p' ../include/gethin.hpp | xargs -i echo "{}" | dos2unix | while read line ; do sed -e '/#/d' -e '/namespace/d' ../include/$$line; done >> gethin.hpp)
	#$(foreach file,$(HEADER_FILES),sed -e '/#/d' -e '/namespace/d' $(file) >> gethin.hpp;)

.PHONY: single_include-post
single_include-post: single_include-contents
	$(shell echo "}  // namespace gethin" >> gethin.hpp)
	$(shell echo "#endif // GETHIN_HPP" >> gethin.hpp)

.PHONY: single_include-format
single_include-format: single_include-post
	$(shell clang-format -i -style=file gethin.hpp)