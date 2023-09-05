
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
                .ProjectDescription = "Lagrange Interpolation Polynomials",
                .SubmissionDate = "09/08/2023",
                .StudentName = "Arjun Earthperson",
        };
    }

    void run(Output &output, Input &input, boost::program_options::variables_map &values) override {
        std::cout<<"run super\n";
        InLab2::run(output, input, values);
        std::cout<<"run me\n";
    }

};
