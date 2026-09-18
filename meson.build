project('OSSEU demo', 'c',
        license: 'BSD-2-Clause')

# Dependencies: just needed to check for 'wpe-webkit-2.0'.
#
# Note: No need to check for 'wpe-platform-2.0' unless you want to
# be explicit on your WPE installation having WPEPlatform enabled.
wpe_webkit_dep = dependency('wpe-webkit-2.0')

# Main demo..
executable('wpe-demo', 'wpe-demo.c',
    dependencies: [
        wpe_webkit_dep
    ])

# Simpler demo from the slides (no signals handled).
executable('wpe-demo-simple', 'wpe-demo-simple.c',
    dependencies: [
        wpe_webkit_dep
    ])
