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


PostBuild.glfw.Debug:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglfw3.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglfw3.a


PostBuild.imgui.Debug:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libimgui.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libimgui.a


PostBuild.libnoise.Debug:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/liblibnoise.a


PostBuild.Project_Polly.Debug:
PostBuild.glfw.Debug: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly
PostBuild.glew.Debug: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly
PostBuild.glm.Debug: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly
PostBuild.imgui.Debug: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly
PostBuild.libnoise.Debug: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly:\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglfw3.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglew.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libimgui.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/liblibnoise.a
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/Project_Polly


PostBuild.glew.Release:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglew.a


PostBuild.glfw.Release:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglfw3.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglfw3.a


PostBuild.imgui.Release:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libimgui.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libimgui.a


PostBuild.libnoise.Release:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/liblibnoise.a


PostBuild.Project_Polly.Release:
PostBuild.glfw.Release: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly
PostBuild.glew.Release: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly
PostBuild.glm.Release: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly
PostBuild.imgui.Release: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly
PostBuild.libnoise.Release: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly:\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglfw3.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglew.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libimgui.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/liblibnoise.a
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/Project_Polly


PostBuild.glew.MinSizeRel:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglew.a


PostBuild.glfw.MinSizeRel:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglfw3.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglfw3.a


PostBuild.imgui.MinSizeRel:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libimgui.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libimgui.a


PostBuild.libnoise.MinSizeRel:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/liblibnoise.a


PostBuild.Project_Polly.MinSizeRel:
PostBuild.glfw.MinSizeRel: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly
PostBuild.glew.MinSizeRel: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly
PostBuild.glm.MinSizeRel: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly
PostBuild.imgui.MinSizeRel: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly
PostBuild.libnoise.MinSizeRel: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly:\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglfw3.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglew.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libimgui.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/liblibnoise.a
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/Project_Polly


PostBuild.glew.RelWithDebInfo:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglew.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglew.a


PostBuild.glfw.RelWithDebInfo:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglfw3.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglfw3.a


PostBuild.imgui.RelWithDebInfo:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libimgui.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libimgui.a


PostBuild.libnoise.RelWithDebInfo:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/liblibnoise.a:
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/liblibnoise.a


PostBuild.Project_Polly.RelWithDebInfo:
PostBuild.glfw.RelWithDebInfo: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly
PostBuild.glew.RelWithDebInfo: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly
PostBuild.glm.RelWithDebInfo: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly
PostBuild.imgui.RelWithDebInfo: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly
PostBuild.libnoise.RelWithDebInfo: /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly:\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglfw3.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglew.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libimgui.a\
	/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/liblibnoise.a
	/bin/rm -f /Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/Project_Polly




# For each target create a dummy ruleso the target does not have to exist
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglew.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libglfw3.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/libimgui.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Debug/liblibnoise.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglew.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libglfw3.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/libimgui.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/MinSizeRel/liblibnoise.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglew.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libglfw3.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/libimgui.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/RelWithDebInfo/liblibnoise.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglew.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libglfw3.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/libimgui.a:
/Users/kathleen/Desktop/projectPolly/Project_Polly/bin/Release/liblibnoise.a:
