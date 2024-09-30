#include <iostream>
#include <vector>

using namespace std;
using Matrix = vector<vector<double>>;
using Channels = vector<Matrix>; // list of channels => filter or output
using Filters = vector<Channels>; // list of filters

// Function to perform convolution across multiple channels and filters
Channels conv_multi_channel(const Channels& input_channels, const Filters& filters) {
    Channels outputs(filters.size());
    for (int f = 0; f < filters.size(); ++f) { // for each filter
        // Initialize output matrix for this filter with zeros
        int oRows = input_channels[0].size() - filters[f][0].size() + 1;
        int oCols = input_channels[0][0].size() - filters[f][0][0].size() + 1;
        outputs[f] = Matrix(oRows, vector<double>(oCols, 0.0));

        // Convolution operation on input channels and corresponding filter channels
        for (int c = 0; c < input_channels.size(); ++c) {
            int iRows = input_channels[c].size();
            int iCols = input_channels[c][0].size();
            int fRows = filters[f][c].size();
            int fCols = filters[f][c][0].size();

            vector<double> inputArray(iRows * iCols);
            vector<double> filterArray(fRows * fCols);

            for (int i = 0; i < iRows; ++i) {
                for (int j = 0; j < iCols; ++j) {
                    inputArray[i * iCols + j] = input_channels[c][i][j];
                }
            }

            for (int fi = 0; fi < fRows; ++fi) {
                for (int fj = 0; fj < fCols; ++fj) {
                    filterArray[fi * fCols + fj] = filters[f][c][fi][fj];
                }
            }
            
            for (int i = 0; i < oRows; ++i) {
                for (int j = 0; j < oCols; ++j) {
                    double sum = 0.0;
                    for (int fi = 0; fi < fRows; ++fi) {
                        for (int fj = 0; fj < fCols; ++fj) {
                            sum += inputArray[(i + fi) * iCols + (j + fj)] * filterArray[fi * fCols + fj];
                        }
                    }
                    outputs[f][i][j] += sum;
                }
            }

            // vector<double> outputArray(oRows * oCols);
            // for (int i = 0; i< oRows*oCols; ++i){
            //     double sum = 0.0;
            //     for (int fi = 0 ; fi < fRows*fCols; ++fi){
            //         sum += inputArray[((int (fi/fCols)+ int(i/oCols))*iCols)+(fi%fCols)+(i%oCols)]* filterArray[fi];
            //     }
            //     outputArray[i] += sum;
            // }

            // for (int i = 0; i< oRows*oCols; ++i){
            //     outputs[f][int(i/oCols)][i%oCols] = outputArray[i];
            // }


            

            // Convolution operation => 1 input channel * 1 filter channel
            // for (int i = 0; i < oRows; ++i) {
            //     for (int j = 0; j < oCols; ++j) {
            //         double sum = 0.0;
            //         for (int fi = 0; fi < fRows; ++fi) {
            //             for (int fj = 0; fj < fCols; ++fj) {
            //                 sum += input_channels[c][i + fi][j + fj] * filters[f][c][fi][fj];
            //             }
            //         }
            //         outputs[f][i][j] += sum;
            //     }
            // }
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

    Channels output = conv_multi_channel(input_channels, filters);
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