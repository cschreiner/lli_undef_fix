lli_undef_fix
=============

Project Description:
Create a variant of LLVM's LLI interpreter that avoids undefined behaviors.

Notes
=====
Initial checkouts were performed as follows:

svn co -r 217983  http://llvm.org/svn/llvm-project/llvm/trunk git/llvm
svn co -r217983 http://llvm.org/svn/llvm-project/test-suite/trunk test-suite/trunk git/test-suite

Initial build is done via the directions in the "Compiling the LLVM Suite
Source Code" instructions in

http://www.llvm.org/docs/GettingStarted.html#checkout-llvm-from-subversion

I created a front end for the `configure` script that records the options
passed to it, that is git/dbin/configure-llvm script.  Always run the front
end instead of running `configure` directly.


end of file
===========
