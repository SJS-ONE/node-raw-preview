{
    "targets": [
        {
            'variables': {
                'pkg-config': 'pkg-config' 
            },
            "target_name": "native",
            "sources": [
                "binding.cpp",
                "base64/base64.cpp"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                
            ],
            "dependencies": [
                "<!(node -p \"require('node-addon-api').gyp\")",
            ],
            'ldflags': [
                '<!@(<(pkg-config) --libs-only-L --libs-only-other exiv2)',
            ],
            'cflags': [
                '<!@(<(pkg-config) --cflags exiv2)',
            ],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            'libraries': [
                '<!@(<(pkg-config) --libs-only-l exiv2)',
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