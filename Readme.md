# [BlurHash](http://blurha.sh)

BlurHash is a compact representation of a placeholder for an image.

## Instruction

This is a Debian package binding of [the original repo](https://github.com/woltapp/blurhash).

## How does it work?

In short, BlurHash takes an image, and gives you a short string (only 20-30 characters!) that represents the placeholder for this
image. You do this on the backend of your service, and store the string along with the image. When you send data to your
client, you send both the URL to the image, and the BlurHash string. Your client then takes the string, and decodes it into an
image that it shows while the real image is loading over the network. The string is short enough that it comfortably fits into
whatever data format you use. For instance, it can easily be added as a field in a JSON object.

## Debian packaging implementation

This code implements an encoder for the BlurHash algorithm in C. It can be used to integrate into other language
using an FFI interface. Currently the Python integration uses this code.

### Usage as a library

Include the `encode.c` and `encode.h` files in your project. They have no external dependencies.

A single file function is defined:

    const char *blurHashForPixels(int xComponents, int yComponents, int width, int height, uint8_t *rgb, size_t bytesPerRow) {

This function returns a string containing the BlurHash. This memory is managed by the function, and you should not free it.
It will be overwritten on the next call into the function, so be careful!

* `xComponents` - The number of components in the X direction. Must be between 1 and 9. 3 to 5 is usually a good range for this.
* `yComponents` - The number of components in the Y direction. Must be between 1 and 9. 3 to 5 is usually a good range for this.
* `width` - The width in pixels of the supplied image.
* `height` - The height in pixels of the supplied image.
* `rgb` - A pointer to the pixel data. This is supplied in RGB order, with 3 bytes per pixels.
* `bytesPerRow` - The number of bytes per row of the RGB pixel data.

### Usage as a command-line tool

You can also build a command-line version to test the encoder and decoder. However, note that it uses `stb_image` to load images,
which is not really security-hardened, so it is **not** recommended to use this version in production on untrusted data!
Use one of the integrations instead, which use more robust image loading libraries.

Nevertheless, if you want to try it out quickly, simply run:

	$ make blurhash_encoder
	$ ./blurhash_encoder 4 3 ../Swift/BlurHashTest/pic1.png
	LaJHjmVu8_~po#smR+a~xaoLWCRj

If you want to try out the decoder, simply run:

	$ make blurhash_decoder
	$ ./blurhash_decoder "LaJHjmVu8_~po#smR+a~xaoLWCRj" 32 32 decoded_output.png

## Authors

* [Dag Ågren](https://github.com/DagAgren) - Original algorithm design, Swift and C implementations
* [Mykhailo Shchurov](https://github.com/shchurov) - Kotlin decoder implementation
* [Hang Duy Khiem](https://github.com/hangduykhiem) - Android demo app
* [Olli Mahlamäki](https://github.com/omahlama) - TypeScript decoder and encoder implementations
* [Atte Lautanala](https://github.com/lautat) - Python integration
* [Lorenz Diener](https://github.com/halcy) - Pure Python implementation
* [Boris Momčilović](https://github.com/kornrunner) - Pure PHP implementation
* [Hendrik Schnepel](https://github.com/hsch) - Java encoder implementation
* [Tuomo Virolainen](https://github.com/tvirolai) - Clojure implementation
* [Fotis Papadogeorgopoulos](https://github.com/fpapado) - Rust and WebAssembly implementation
* [Sam Protas](https://github.com/SamProtas) - Pure Haskell implementation
* [Markus Sammallahti](https://github.com/markussammallahti) - Scala implementation
* [Kacper Perzankowski](https://github.com/perzanko) - Elixir encoder implementation
* [Belvi Nosakhare](https://github.com/KingsMentor/BlurHashExt) - Kotlin extensions of Blurhash for ImageView, Glide, and Piccasso optimized for Android.
* [Armedi](https://github.com/armedi) - ReScript (BuckleScript) implementation.
* [Ricardo Cruz](https://github.com/piradoiv) - Xojo implementation.
* [Marc Rousavy](https://github.com/mrousavy) - React Native UI Component
* [Matt Howard](https://github.com/mhoward540) - Zig implementation
* [Hannu Hartikainen](https://github.com/dancek) - BQN implementation
* [Wajahat Iqbal](https://github.com/wajahat-iqbal) - Jetpack compose-based implementation optimized for using any component as a painter.
* [Fumiama Minamoto](https://github.com/fumiama) - Debian packaging of C implementation.


* _Your name here?_
