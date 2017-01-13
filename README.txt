README:

License: MIT

For Building and Running PSAT in Windows:
	Pre-requisites: * Make sure one has the AMO-Tools-Suit, 
			  and that it is on the same directory level as PSAT.
			* Python 2.7
			* npm
			* Node.js
			* Electron
			* Git

	Step 1: Open A terminal window and navigate to where PSAT is.
	Step 2: Run the win.cmd to build the program.
	Step 3: Enter "npm run pkg" to create the application.
	Step 4: The app will be found next to one's PSAT folder and go under the name releases,
		which will have PSAT--win32-x64 in it, which in turn will have the PSAT Application.

For Building and Running PSAT in MAC OS:
	Pre-requisites: * Make sure one has the AMO-Tools-Suit, 
			  and that it is on the same directory level as PSAT.
			* Python 2.7
			* npm
			* Node.js
			* Electron
			* Git	

	Step 1: Open A terminal window and navigate to where PSAT is.
	Step 2: Run ./mac command.
	Step 3: Enter "npm run pkg" to create the application.
	Step 4: The app will be found next to one's PSAT folder and
		go under the name releases, which will have PSAT—darwin-x64
		in it, which in turn will have the PSAT Application.

Building PSAT in Linux:
	Tools needed:
		git, npm, nodejs, electron

	1. Clone AMO-Tools-Suite to project directory with git
	2. Clone PSAT to project directory with git (NOTE: the AMO-Tools-Suite and PSAT folders MUST be in the same directory)
	3. Open a terminal, and navigate to AMO-Tools-Suite
	4. Run cmake: cmake path/to/AMO-Tools-Suite
	*NOTE* As of now, CMakelists.txt needs to be edited to run cmake correctly. ${CMAKE_DL_LIBS} needs to be added to the tartet_link_library() call.
	5. Run make: make
	6. Navigate to PSAT
	7. Run the mac script: ./mac
	8. Run electron-packager: electron-packager ./
	9. The executable program will be located in the PSAT-linux-x64 folder which is inside the PSAT folder.