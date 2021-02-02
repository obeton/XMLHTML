Rationale:

   In working on this project, I tried to implement a solution twice.

   The first solution was simply trying to connect two different third party libraries (pugixml and CTML) to produce the desired results. All I really added was some in between structure to isolate the libraries from each other for easier unit testing.

   The second solution was trying make a more OOD implementation. So the goal of this effort was to more cleanly separate importing a text file from exporting to another text file. To achieve this I created two abstract classes, TextSource and TextUser. 

   TextSource:
      -Provides an interface (via Element) for other objects to access TextUser owned data.
	      -Does not expose or reveal the structure of the underlying data
              -If underlying data has fields added/removed, there is no modification required for public interface (i.e., shared structs or "getters" do not have to be modified). 
              -Less performant than initial solution, more complicated and extra indirection (TextUser does not directly provide data access)
              -It is more agnostic to underlying data (as long as corresponding enumerations exist) and underlying text access can be searched using iterator-like mechanism

   TextUser:
      -Takes any TextSource and using the element interface can import data from TextSource

   These two classes worked to encapsulate their respective functionality so that should other text formats be added for importing/exporting to/from or different data representations (i.e., DVDs, Books) it should be pretty straightforward to implement without affecting current functionality or major refactor.

   From these two classes, I made two concrete implementations, XMLImport(TextSource) and HTMLExport(TextUser). HTMLExport operates on XMLImport to get the relevant data into its HTML table without any knowledge of the internals of XMLImport.

   Overall, the largest tradeoff was encapsulation vs. performance. Making text elements "iterable" is not necessarily straightforward, so this design choice is mitigated by my use of pugixml's XPATH functionality. Generally it will take more effort to get the text "iterable" either by searching through raw string or loading text into structures/classes.

   Two enhancements that I can think of making are:
      1) Dynamically generate recordIds and fieldIds, given some configuration that specifies what data we want from input, the fieldIds and tag text could be generated at runtime to avoid the need to know the structure of the data at compile-time.
      2) Currently works as 1 input file - 1 output file. Could be possible to support multiple input files (that have different format even) to single output file

Notes:
   When Building own binaries: I didn't use CMAKE or anything sophisticated. Pretty much just compiled all the *.cpp files together. To alternate between generating the Catch2 testing main and my own main I used a preprocessor flag in BuildTests.hpp. If it's defined, then the Catch2 main is used otherwise it uses the main in main.cpp. I compiled using msvc on Windows but I tried to use only STL classes for my own file access and avoided using any microsoft/winapi specific functions. However, I can't speak to how it would run/build on other environments.

Third Party Libraries Used:
   Catch2 - c++ unit testing library @ https://github.com/catchorg/Catch2
   pugixml - c++ xml library @ https://github.com/zeux/pugixml
   CTML - c++ html library @ https://github.com/tinfoilboy/CTML