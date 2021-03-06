function lanelet = import_lanelet_txt(filename, dataLines)
%IMPORTFILE Import data from a text file
%  LOOPGEORHD1 = IMPORTFILE(FILENAME) reads data from text file FILENAME
%  for the default selection.  Returns the data as a table.
%
%  LOOPGEORHD1 = IMPORTFILE(FILE, DATALINES) reads data for the
%  specified row interval(s) of text file FILENAME. Specify DATALINES as
%  a positive scalar integer or a N-by-2 array of positive scalar
%  integers for dis-contiguous row intervals.
%
%  Example:
%  loopgeoRHD1 = importfile("/home/is18902/Downloads/loop_geo_RHD.txt", [2, Inf]);
%
%  See also READTABLE.
%
% Auto-generated by MATLAB on 01-Mar-2021 10:15:00

%% Input handling

% If dataLines is not specified, define defaults
if nargin < 2
    dataLines = [2, Inf];
end

%% Setup the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 4);

% Specify range and delimiter
opts.DataLines = dataLines;
opts.Delimiter = ",";

% Specify column names and types
opts.VariableNames = ["lanelet_id", "bound", "x", "y"];
opts.VariableTypes = ["double", "categorical", "double", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, "bound", "EmptyFieldRule", "auto");

% Import the data
lanelet = readtable(filename, opts);

end