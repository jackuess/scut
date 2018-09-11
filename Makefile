SOURCES = scut.c scut.h

.PHONY: indent
indent:
	clang-format -i -style=file $(SOURCES)
