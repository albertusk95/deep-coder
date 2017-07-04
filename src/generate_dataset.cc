#include <iostream>
#include <algorithm>
#include <string>
#include "dsl/utils.h"
#include "dataset-generator.h"
#include "attribute.h"

using namespace std;
using namespace dsl;

void output_value(const Value &value) {
    if (value.integer()) {
        cout << value.integer().value();
    } else {
        auto l = value.list().value();
        cout << "[";
        for (auto i = 0; i < l.size(); i++) {
            auto x = l.at(i);

            cout << x;
            if (i != (l.size() - 1)) {
                cout << ",";
            }
        }
        cout << "]";
    }
}
void output_input(const Input &input) {
    cout << "[";
    for (auto i = 0; i < input.size(); i++) {
        auto x = input.at(i);

        output_value(x);
        if (i != (input.size() - 1)) {
            cout << ",";
        }
    }
    cout << "]";
}

void output_attribute(const Attribute &attr) {
    std::vector<double> vec = attr;
    cout << "[";
    for (auto i = 0; i < vec.size(); i++) {
        cout << vec.at(i);
        if (i != (vec.size() - 1)) {
            cout << ",";
        }
    }
    cout << "]";
}

int main(int argc, char **argv) {
    size_t max_length = 4;
    size_t dataset_size = 0;

    if (argc >= 2) {
        max_length = atoi(argv[1]);
    }
    if (argc >= 3) {
        dataset_size = atoi(argv[2]);
    }

    cerr << "Generate dataset\n" << "  Max-Length: " << max_length << "\n  Dataset-Size: " << dataset_size << endl;
    auto dataset = generate_dataset(2, max_length, dataset_size);

    cout << "[\n";
    if (dataset) {
        auto x = dataset.value();
        long long int cnt = 0;
        for (const auto &p: x.programs) {
            cnt += 1;
            for (auto i = 0; i < p.second.size(); ++i) {
                const auto &data = p.second.at(i);
                const auto &program = data.first;
                const auto &examples = data.second;
                auto attribute = Attribute(program);

                cerr << "# Program\n" << program << flush;
                cout << "{\"examples\":[\n";
                for (auto j = 0; j < examples.size(); ++j) {
                    const auto &example = examples.at(j);

                    cout << "{\"input\":";
                    output_input(example.input);
                    cout << ",\"output\":";
                    output_value(example.output);
                    cout << "}";
                    if (j != examples.size() - 1) {
                        cout << ",";
                    }
                    cout << "\n";
                }
                cout << "],\n\"attribute\":";
                output_attribute(attribute);

                cout << "}";
                if (cnt != x.programs.size() ||
                        i != (p.second.size() - 1)) {
                    cout << ",";
                }
                cout << "\n" << flush;
            }
        }
    } else {
        cerr << "Fail to generate dataset" << endl;
    }
    cout << "]" << endl;

    return 0;
}