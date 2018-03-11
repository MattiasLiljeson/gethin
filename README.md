gethin
======
A cross-platform header-only c++11 getopt library

[![Build Status](https://travis-ci.org/MattiasLiljeson/gethin.svg?branch=master)](https://travis-ci.org/MattiasLiljeson/gethin)

Usage
-----
There are three types of options you can use, Strings, Flags and Sets. Create
the options you need. Create a OptionReader object with the options supplied in
a vector and call the read method on the OptionReader object with argc and
argv. Call the value method on the options to get the parsed arguments.

Example:
```
	String f; f.mandatory( false ).name("argument name, shown in help")
		.shortOpt( 'f' ).longOpt( "foo" )
		.help( "some help text about foo" );

    Flag b; b.shortOpt( 'b' ).longOpt( "bar" )
		.help( "some help text about bar" );

	Set pga; pga.mandatory( false )
		.alternatives( vector<string>{ "kmeans", "iterative", "histogram", "mixed" } )
		.shortOpt( 'a' ).longOpt( "palette-generation-algorithm" )
		.help( "Specify 'kmeans', 'iterative', 'histogram' or 'mixed' where 'mixed' "
			"combines the results of 'histogram' and 'iterative'" );

    OptionReader optReader( vector<Parameter*>{&f, &b, &pga} );
```
