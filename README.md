# WPE WebKit launcher demo for OSS EU 2026

A small web launcher built on [WPE WebKit](https://wpewebkit.org) and the [new WPEPlatform API](https://wpewebkit.org/about/faq.html#what-is-the-wpeplatform-api%3F).


It was written as the demo for the talk [WPE Hands-On: Writing a Launcher for Embedded Devices with the New WPEPlatform API](https://osselceu2026.sched.com/event/2RadA/wpe-hands-on-writing-a-launcher-for-embedded-devices-with-the-new-wpeplatform-api-mario-sanchez-prada-igalia), presented at the Open Source Summit Europe 2026 in Prague, on October 8th 2026.

## Building

You need `meson`, `ninja` and a WPE WebKit installation built with WPEPlatform support (i.e. `wpe-webkit-2.0` and `wpe-platform-2.0` pkg-config modules).
You can check their existence with:
```sh
pkg-config --exists wpe-webkit-2.0
pkg-config --exists wpe-platform-2.0
```

If WPE is installed in a custom prefix, point `pkg-config` at it first:
```sh
export PKG_CONFIG_PATH=/path/to/prefix/lib/pkgconfig:$PKG_CONFIG_PATH
```

Build the sources with:
```sh
meson setup BUILD
ninja -C BUILD
```

## Running

```sh
./BUILD/wpe-demo [URL]
```

If you don't pass a URL, it loads https://wpewebkit.org.

With WPE in a custom prefix, you may also need to prepend `/path/to/prefix/lib` to `LD_LIBRARY_PATH`.

Keyboard shortcuts:

| Shortcut | Action            |
|----------|-------------------|
| Ctrl+R   | Reload            |
| Ctrl+F   | Toggle fullscreen |
| Ctrl+M   | Toggle maximize   |
| Ctrl+Q   | Quit              |

## License

BSD 2-Clause. See [LICENSE](LICENSE).
