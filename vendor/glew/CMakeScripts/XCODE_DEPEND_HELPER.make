# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.glew.Debug:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglew.a


PostBuild.glew.Release:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglew.a


PostBuild.glew.MinSizeRel:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglew.a


PostBuild.glew.RelWithDebInfo:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglew.a




# For each target create a dummy ruleso the target does not have to exist
