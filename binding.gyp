{
  	"targets": [
		{
	    	"target_name": "bridge",
	    	'include_dirs': ['../AMO-Tools-Suite/include'],
			'sources' : [
				'bridge.cpp',
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/').map(f=>'../AMO-Tools-Suite/src/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/calculator/').map(f=>'../AMO-Tools-Suite/src/calculator/'+f).join(' '))\")"
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
