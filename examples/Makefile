CFLAGS  += -std=c99 -I .. -Wall -Wextra -Wundef -Wshadow -Wcast-align -Werror -g
LDFLAGS += -L .. -l:libxxhash.a
FILES    = ../cli/COPYING

.PHONY: default
default: all

.PHONY: all
all: build-all test-all


build-all:			\
	xxhash-hello		\
	xxh32-one-shot		\
	xxh64-one-shot		\
	xxh3-64bits-one-shot	\
	xxh3-128bits-one-shot	\
	xxh32-stream		\
	xxh64-stream		\
	xxh3-64bits-stream	\
	xxh3-128bits-stream	\


test-all:
	./xxhash-hello
	./xxh32-one-shot
	./xxh64-one-shot
	./xxh3-64bits-one-shot
	./xxh3-128bits-one-shot
	./xxh32-stream        $(FILES)
	./xxh64-stream        $(FILES)
	./xxh3-64bits-stream  $(FILES)
	./xxh3-128bits-stream $(FILES)


clean:
	@$(RM) ./xxhash-hello
	@$(RM) ./xxh32-one-shot
	@$(RM) ./xxh64-one-shot
	@$(RM) ./xxh3-64bits-one-shot
	@$(RM) ./xxh3-128bits-one-shot
	@$(RM) ./xxh32-stream
	@$(RM) ./xxh64-stream
	@$(RM) ./xxh3-64bits-stream
	@$(RM) ./xxh3-128bits-stream


format-code:
	clang-format -i xxhash-hello.c
	clang-format -i xxh32-one-shot.c
	clang-format -i xxh64-one-shot.c
	clang-format -i xxh3-64bits-one-shot.c
	clang-format -i xxh3-128bits-one-shot.c
	clang-format -i xxh32-stream.c
	clang-format -i xxh64-stream.c
	clang-format -i xxh3-64bits-stream.c
	clang-format -i xxh3-128bits-stream.c


xxhash-hello:          xxhash-hello.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh32-one-shot:        xxh32-one-shot.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh64-one-shot:        xxh64-one-shot.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh3-64bits-one-shot:  xxh3-64bits-one-shot.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh3-128bits-one-shot: xxh3-128bits-one-shot.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh32-stream:          xxh32-stream.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh64-stream:          xxh64-stream.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh3-64bits-stream:    xxh3-64bits-stream.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

xxh3-128bits-stream:   xxh3-128bits-stream.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)
