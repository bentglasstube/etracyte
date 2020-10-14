UNAME=$(shell uname)
ifeq ($(UNAME), Windows)
	CROSS=x86_64-w64-mingw32.static-
endif

SOURCES=$(wildcard *.cc) $(wildcard gam/*.cc)
CONTENT=$(wildcard content/*.png) $(wildcard content/*.ogg) $(wildcard content/*.wav)
ICONS=icon.png
BUILDDIR=$(CROSS)output
OBJECTS=$(patsubst %.cc,$(BUILDDIR)/%.o,$(SOURCES))
NAME=etracyte
VERSION=$(shell git describe --tags --dirty)

CC=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-O3 --std=c++17 -Wall -Wextra -Werror -pedantic -I gam -DNDEBUG
EMFLAGS=-s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_OGG=1 -s USE_VORBIS=1 -s ALLOW_MEMORY_GROWTH=1 -fno-rtti -fno-exceptions
EXTRA=

EXECUTABLE=$(BUILDDIR)/$(NAME)

ifeq ($(UNAME), Windows)
	PACKAGE=$(NAME)-windows-$(VERSION).zip
	LDFLAGS=-static-libstdc++ -static-libgcc
	LDLIBS=`$(PKG_CONFIG) sdl2 SDL2_mixer SDL2_image --cflags --libs` -Wl,-Bstatic
	EXECUTABLE=$(BUILDDIR)/$(NAME).exe
	EXTRA=$(BUILDDIR)/icon.res.o
endif
ifeq ($(UNAME), Linux)
	PACKAGE=$(NAME)-linux-$(VERSION).AppImage
	LDFLAGS=-static-libstdc++ -static-libgcc
	LDLIBS=`$(PKG_CONFIG) sdl2 SDL2_mixer SDL2_image --cflags --libs` -Wl,-Bstatic
endif
ifeq ($(UNAME), Darwin)
	PACKAGE=$(NAME)-osx-$(VERSION).tgz
	LDLIBS=-framework SDL2 -framework SDL2_mixer -framework SDL2_image -rpath @executable_path/../Frameworks
	CFLAGS+=-mmacosx-version-min=10.9
endif

all: $(EXECUTABLE)

echo:
	@echo "Content: $(CONTENT)"
	@echo "Sources: $(SOURCES)"
	@echo "Uname: $(UNAME)"
	@echo "Package: $(PACKAGE)"
	@echo "Version: $(VERSION)"

run: $(EXECUTABLE)
	./$(EXECUTABLE)

video: $(BUILDDIR)/$(NAME).mkv

$(BUILDDIR)/$(NAME).mkv: $(BUILDDIR)/$(NAME).glc $(BUILDDIR)/$(NAME).wav
	glc-play $< -o - -y 1 |ffmpeg -i - -i $(NAME).wav -acodec flac --vcodec libx264 -y $@

$(BUILDDIR)/$(NAME).wav: $(BUILDDIR)/$(NAME).glc
	glc-play $< -a 1 -o $@

$(BUILDDIR)/$(NAME).glc: $(EXECUTABLE)
	glc-capture -pso $@ $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(EXTRA)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(EXTRA) $(LDLIBS)

$(BUILDDIR)/%.o: %.cc
	@mkdir -p $(BUILDDIR)/gam
	$(CC) -c $(CFLAGS) -o $@ $<

package: $(PACKAGE)

$(BUILDDIR)/icon.res.o: $(BUILDDIR)/icon.rc
	$(CROSS)windres $< -O coff $@

$(BUILDDIR)/icon.rc: $(BUILDDIR)/icon.ico
	echo "420 ICON $<" > $@

$(BUILDDIR)/icon.ico: $(ICONS)
	convert $< $@

wasm: $(NAME)-$(VERSION).html

web: wasm $(NAME)-$(VERSION).js $(NAME)-$(VERSION).data
	mkdir -p $(NAME)-web-$(VERSION)
	cp $(NAME)-$(VERSION).js $(NAME)-web-$(VERSION)
	cp $(NAME)-$(VERSION).wasm $(NAME)-web-$(VERSION)
	cp $(NAME)-$(VERSION).data $(NAME)-web-$(VERSION)
	cp $(NAME)-$(VERSION).html $(NAME)-web-$(VERSION)/index.html

$(NAME)-osx-$(VERSION).tgz: $(NAME).app
	mkdir $(NAME)
	cp -r $(NAME).app $(NAME)/.
	tar zcf $@ $(NAME)
	rm -rf $(NAME)

$(NAME)-windows-$(VERSION).zip: $(EXECUTABLE) $(CONTENT)
	mkdir -p $(NAME)/content
	cp $(EXECUTABLE) $(NAME)/`basename $(EXECUTABLE)`
	cp $(CONTENT) $(NAME)/content/.
	zip -r $@ $(NAME)
	rm -rf $(NAME)

$(NAME)-$(VERSION).html: $(SOURCES) $(CONTENT)
	emcc $(CFLAGS) $(EMFLAGS) -o $@ $(SOURCES) --preload-file content/

$(NAME).app: $(EXECUTABLE) launcher $(CONTENT) Info.plist
	rm -rf $(NAME).app
	mkdir -p $(NAME).app/Contents/{MacOS,Frameworks}
	cp $(EXECUTABLE) $(NAME).app/Contents/MacOS/game
	cp launcher $(NAME).app/Contents/MacOS/launcher
	cp -R content $(NAME).app/Contents/MacOS/content
	cp Info.plist $(NAME).app/Contents/Info.plist
	cp -R /Library/Frameworks/SDL2.framework $(NAME).app/Contents/Frameworks/SDL2.framework
	cp -R /Library/Frameworks/SDL2_mixer.framework $(NAME).app/Contents/Frameworks/SDL2_mixer.framework
	cp -R /Library/Frameworks/SDL2_image.framework $(NAME).app/Contents/Frameworks/SDL2_image.framework

$(NAME)-linux-$(VERSION).AppDir: $(EXECUTABLE) $(CONTENT) AppRun icon.png $(NAME).desktop
	rm -rf $@
	mkdir -p $@/usr/{bin,lib}
	mkdir -p $@/content
	cp $(EXECUTABLE) $@/usr/bin
	cp AppRun $@/.
	cp $(NAME).desktop $@/.
	cp icon.png $@/.
	cp $(CONTENT) $@/content/.
	cp /usr/lib/libSDL2{,_image,_mixer}-2.0.so.0 $@/usr/lib/.

$(NAME)-linux-$(VERSION).AppImage: $(NAME)-linux-$(VERSION).AppDir
	ARCH=x86_64 appimagetool $< $@

clean:
	rm -rf $(BUILDDIR)

distclean: clean
	rm -rf *.app *.tgz *.zip
	rm -rf *.AppDir *.AppImage
	rm -rf *.html *.js *.data *.wasm
	rm -rf *-web-*/ *output/

.PHONY: all echo clean distclean run package wasm web video
