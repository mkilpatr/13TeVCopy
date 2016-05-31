# EstTools

This set of scripts is designed for quick implementation of background predictions, as well as making various kinds of plots. 
An example can be found in `LLBEstimator.C`. 

To run it, just compile it in ROOT by <br>
`.L LLBEstimator.C+`
and call the functions.

## Brief description of the files

`Style.hh`
Set the styles for plotting, e.g., color maps, TDR style, etc.

`Quantity.h`
Class handling simple error propagation. Basic operators `+-*/` are overloaded for the `Quantity` class and `std::vector<Quantity>` to take into account the error propagation (*without correlation*). 

`Config.h`
Define classes dealing with events categorization (`Category`), binning (`BinInfo`), samples (`Sample`). They are organized together in `BaseConfig` which will be used by the `Estimator` classes. 

For more details, please read the in-line comments.

`CommonParameters.hh`
Header file storing common selection strings, event categorization, samples, configs, etc.  

`Estimator.hh`
Define the base class for background estimation `BaseEstimator`, with some convenient functions to make comparison plots or data/MC plots for multiple categories.

Each background estimation should derive from the `BaseEstimator` class and implement the actual prediction.

For more details, please read the in-line comments.

`EstHelper.hh`
Various (frequently used) helper functions for getting yields (as `Quantity`), histograms, and making comparison plots or Data/MC plots.

`MiniTools.hh`
Various (less frequently used) helper functions developed over time. *Beware some of them are not well tested.*

