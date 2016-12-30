#include <otest2/casegenerated.h>
#include <otest2/context.h>
#include <otest2/stategenerated.h>
#include <otest2/suitegenerated.h>

#include <datstr/dstring.h>
#include <iostream>

class ExampleSuite : public ::OTest2::SuiteGenerated {
  private:
    /* -- avoid copying */
    ExampleSuite(
        const ExampleSuite&);
    ExampleSuite& operator =(
        const ExampleSuite&);

  private:
    dstring suite_text;

    class FirstCase : public ::OTest2::CaseGenerated {
      private:
        /* -- avoid copying */
        FirstCase(
            const FirstCase&);
        FirstCase& operator =(
            const FirstCase&);

        dstring& suite_text;
        dstring case_text;

        class Init : public ::OTest2::StateGenerated {
          private:
            /* -- avoid copying */
            Init(
                const Init&);
            Init& operator =(
                const Init&);

            dstring& suite_text;
            dstring& case_text;
            dstring state_text;

            virtual void runState(
                const ::OTest2::Context& context_) {
              std::cout << "Hello world" << std::endl;
              std::cout << suite_text << case_text << state_text << std::endl;
            }

          public:
            explicit Init(
                const ::OTest2::Context& context_,
                ExampleSuite* suite_,
                FirstCase* case_) :
              ::OTest2::StateGenerated(context_),
              suite_text(suite_ -> suite_text),
              case_text(case_ -> case_text),
              state_text("State") {

            }

            virtual ~Init() {

            }
        };

      public:
        explicit FirstCase(
            const ::OTest2::Context& context_,
            ExampleSuite* suite_) :
          ::OTest2::CaseGenerated(context_),
          suite_text(suite_ -> suite_text),
          case_text("Case") {

        }

        virtual ~FirstCase() {

        }
    };

  public:
    explicit ExampleSuite(
        const ::OTest2::Context& context_) :
      ::OTest2::SuiteGenerated(context_),
      suite_text("Suite") {

    }

    virtual ~ExampleSuite() {

    }
};
