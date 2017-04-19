{
  	"targets": [
		{
	    	"target_name": "bridge",
	        'include_dirs': ['../AMO-Tools-Suite/include', '../AMO-Tools-Suite/include/calculator', '../AMO-Tools-Suite/include/calculator/losses', '../AMO-Tools-Suite/include/calculator/motor', '../AMO-Tools-Suite/include/calculator/pump', '../AMO-Tools-Suite/include/calculator/util', '../AMO-Tools-Suite/include/phast', '../AMO-Tools-Suite/include/psat'],
			'sources' : [
				'bridge.cpp',
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/').map(f=>'../AMO-Tools-Suite/src/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/calculator/').map(f=>'../AMO-Tools-Suite/src/calculator/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/calculator/losses').map(f=>'../AMO-Tools-Suite/src/calculator/losses/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/calculator/motor').map(f=>'../AMO-Tools-Suite/src/calculator/motor/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/calculator/pump').map(f=>'../AMO-Tools-Suite/src/calculator/pump/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/calculator/util').map(f=>'../AMO-Tools-Suite/src/calculator/util/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/psat/').map(f=>'../AMO-Tools-Suite/src/psat/'+f).join(' '))\")",
				"<!@(node -e \"console.log(require('fs').readdirSync('../AMO-Tools-Suite/src/phast/').map(f=>'../AMO-Tools-Suite/src/phast/'+f).join(' '))\")"
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
