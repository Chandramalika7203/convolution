#include <iostream>
#include <vector>

using namespace std;
using Matrix = vector<vector<double>>;
using Channels = vector<Matrix>; // list of channels => filter or output
using Filters = vector<Channels>; // list of filters

// Function to perform convolution across multiple channels and filters
Channels conv_multi_channel(const Channels& input_channels, const Filters& filters, int stride, int pad) {
    Channels outputs(filters.size());
    int paddedRows = input_channels[0].size() + 2 * pad;// no. of rows in input channel +2*pad
    int paddedCols = input_channels[0][0].size() + 2 * pad;//no. of cols in inp channel +2*pad

    for (int f = 0; f < filters.size(); ++f) { // for each filter
        // Initialize output matrix for this filter with zeros
        int oRows = (paddedRows - filters[f][0].size()) / stride + 1;
        int oCols = (paddedCols- filters[f][0][0].size()) / stride + 1;
        outputs[f] = Matrix(oRows, vector<double>(oCols, 0.0));

        // Convolution operation on input channels and corresponding filter channels
        
            for (int i = 0; i < oRows; ++i) {
                for (int j = 0; j < oCols; ++j) {
                    double sum = 0.0;
                    for (int c = 0; c < input_channels.size(); ++c) {
                        int iRows = paddedRows;
                        int iCols = paddedCols;
                        int fRows = filters[f][c].size();
                        int fCols = filters[f][c][0].size();

                        vector<double> inputArray(iRows * iCols, 0.0);
                        vector<double> filterArray(fRows * fCols);

                        for (int i = 0; i < input_channels[c].size(); ++i) {
                            for (int j = 0; j < input_channels[c][0].size(); ++j) {
                                inputArray[(pad+i) * iCols + j+pad] = input_channels[c][i][j];
                            }
                        }

                        for (int fi = 0; fi < fRows; ++fi) {
                            for (int fj = 0; fj < fCols; ++fj) {
                                filterArray[fi * fCols + fj] = filters[f][c][fi][fj];
                            }
                        }
                        
                        for (int fi = 0; fi < fRows; ++fi) {
                            for (int fj = 0; fj < fCols; ++fj) {
                                sum += inputArray[(i* stride + fi) * iCols + (j*stride + fj)] * filterArray[fi * fCols + fj];
                            }
                        }
                    outputs[f][i][j] = sum;
                }
            }
        }
    }
    return outputs;
}


int main() {
    Channels input_channels(2); // Sample input with 2 channels
    input_channels[0] = {
        {1, 2, 3, 4, 5, 6},
        {1, 0, 4, 5, 0, 0},
        {6, 2, 2, 4, 0, 6},
        {1, 2, 2, 5, 5, 8},
        {0, 2, 0, 4, 5, 6},
        {1, 4, 3, 4, 5, 0}
    };
    input_channels[1] = {
        {1, -1, 2, -3, -4, 1},
        {1, -8, -4, -5, -1, 1},
        {0, 0, -3, 4, -2, -1},
        {0, 2, 2, -5, 0, -1},
        {0, -2, 1, -4, 0, -1},
        {0, -8, 3, -4, 3, -1}
    };

    Filters filters = { // Each filter here also has 2 channels similar to the number of input channels
        {{{1, 2, 3}, {0, 2, 0}, {1, 0, 3}},
         {{-1, 0, 1}, {0, -1, 2}, {1, 2, 3}}},
        {{{1, 1, 1}, {0, 0, 0}, {1, 0, 1}},
         {{-1, -1, -1}, {0, -1, 1}, {1, 9, 3}}},
        {{{0, 0, 0}, {2, 3, 2}, {3, 3, 3}},
         {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}}
    };

    int stride = 2;
    int pad = 1;

    Channels output = conv_multi_channel(input_channels, filters,stride,pad);
    cout << "Output: " << endl;
    for (int f = 0; f < output.size(); ++f) {
        cout << "Feature Map " << f + 1 << ":" << endl;
        for (auto& row : output[f]) {
            for (double value : row) {
                cout << value << " ";
            }
            cout << endl;
        }
        cout << endl;
    }


    return 0;
}