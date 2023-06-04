{
    "targets": [
        {
            "target_name": "native",
            "sources": [
                "src/binding.cpp",
                "base64/base64.cpp"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",                
                '<!@(pkg-config --cflags-only-I exiv2 libssh | sed s/-I//g)',
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")",
            ],
            "cflags!": ["-fno-exceptions", "-Wno-deprecated-declarations"],
            "cflags_cc!": ["-fno-exceptions", "-Wno-deprecated-declarations"],
            'libraries': [
                '<!@(pkg-config --libs-only-l exiv2 libssh)',
                '<!@(pkg-config --libs-only-L --libs-only-other exiv2 libssh)'
            ],
            "defines": ["NAPI_CPP_EXCEPTIONS"],
            'conditions': [
                ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                }
                }]
            ]
        }
    ]
}