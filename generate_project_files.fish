#!/usr/local/bin/fish

set ofBase ../..
set examplesBase (pwd)

function generateProjectFiles
	echo "Generating Project Files for: $argv "
  eval ./../devApps/projectGenerator/bin/emptyExampleDebug.app/Contents/MacOS/emptyExampleDebug "$examplesBase/$argv";
end

function removeProjectFiles
	echo "Removing Project Files for: $argv "
  	
	# vs2012 files
  	rm -f "$examplesBase/$argv/$argv.sln";
  	rm -f "$examplesBase/$argv/$argv.vcxproj";
  	rm -f "$examplesBase/$argv/$argv.vcxproj.filters";
  	rm -f "$examplesBase/$argv/$argv.vcxproj.user";
  	rm -f "$examplesBase/$argv/icon.rc";
  	
  	# xcode project files
  	rm -Rf "$examplesBase/$argv/$argv.xcodeproj";
  	rm -f "$examplesBase/$argv/openFrameworks-Info.plist";
  	rm -f "$examplesBase/$argv/Project.xcconfig";

  	# codeblocks project files

  	rm -f "$examplesBase/$argv/$argv.cbp";
  	rm -f "$examplesBase/$argv/$argv.workspace";

end


set platforms "--osx --linux64 --vs"

for currentExample in (ls -d */ | cut -f1 -d'/'); # commands; end

	# echo $currentExample
	# removeProjectFiles $currentExample
	generateProjectFiles $currentExample $platforms

end

