{
  	"targets": [
		{
	    	"target_name": "bridge",
	    	'include_dirs': ['../amo-library/include'],
			'sources' : [
				'bridge.cpp',
				"<!@(node -e \"console.log(require('fs').readdirSync('../amo-library/src/').map(f=>'../amo-library/src/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../amo-library/src/calculator/').map(f=>'../amo-library/src/calculator/'+f).join(' '))\")"
			],
			#"cflags": ["-Wall","-std=c++11"],  
			"conditions": [ 
		    	[ 'OS=="mac"', { 
		           	"xcode_settings": {
					    'OTHER_CPLUSPLUSFLAGS' : ['-stdlib=libc++'], 		                
		                #'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'], 
		                'OTHER_LDFLAGS': ['-stdlib=libc++'], 
		                # 'MACOSX_DEPLOYMENT_TARGET': '10.9' 
			   		}
				}]
	       	] 
	    }
	]	
}
