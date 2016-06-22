{
  	"targets": [
		{
	    	"target_name": "api-bridge",
			'sources' : [ 'apibridge.cpp','<!@(ls -1 ../api/*.cpp)','<!@(ls -1 ../api/calculator/*.cpp)'],
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
