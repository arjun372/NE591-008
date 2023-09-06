
#include "../../utils/CommandLine.h"
#include "../../utils/Project.h"

#include "labs/inlab2/inlab2.h"

class OutLab2 : public InLab2 {

public:
    explicit OutLab2(CommandLineArgs args) : InLab2(args) {}

protected:

    HeaderInfo buildHeaderInfo() override {
        return HeaderInfo {
                .ProjectName = "OutLab 02",
                .ProjectDescription = "Lagrange Interpolating Polynomials using Neville's Algorithm",
                .SubmissionDate = "09/08/2023",
                .StudentName = "Arjun Earthperson",
        };
    }

    void run(Dictionary &outputs, Input &inputs, boost::program_options::variables_map &values) override {
        std::cout<<"run super\n";
        InLab2::run(outputs, inputs, values);
        std::cout<<"run me\n";
    }

};
