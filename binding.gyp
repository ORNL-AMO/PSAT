{
  	"targets": [
		{
	    	"target_name": "bridge",
	    	'include_dirs': ['../amo-library'],
			'sources' : [
				'bridge.cpp',
				"<!@(node -e \"console.log(require('fs').readdirSync('../amo-library/').map(f=>'../amo-library/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../amo-library/calculator/').map(f=>'../amo-library/calculator/'+f).join(' '))\")"
			],
			"cflags": ["-Wall","-std=c++11"],  
			"conditions": [ 
		        [ 'OS=="mac"', { 
		            "xcode_settings": { 
		                'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'], 
		                'OTHER_LDFLAGS': ['-stdlib=libc++'], 
		                'MACOSX_DEPLOYMENT_TARGET': '10.7' }
		            }
		        ]
	       	] 
	    }
	]	
}
