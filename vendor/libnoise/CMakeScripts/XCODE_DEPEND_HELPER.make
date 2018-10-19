# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.libnoise.Debug:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/liblibnoise.a


PostBuild.libnoise.Release:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/liblibnoise.a


PostBuild.libnoise.MinSizeRel:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/liblibnoise.a


PostBuild.libnoise.RelWithDebInfo:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/liblibnoise.a




# For each target create a dummy ruleso the target does not have to exist
