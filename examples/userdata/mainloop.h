/*
 * Copyright (C) 2020 Ondrej Starek
 *
 * This file is part of OTest2.
 *
 * OTest2 is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * OTest2 is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OTest2.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OTest2__EXAMPLES_USERDATA_MAINLOOP_H_
#define OTest2__EXAMPLES_USERDATA_MAINLOOP_H_

#include <string>

namespace OTest2 {

class Runner;

namespace Examples {

class MainLoop {
  private:
    struct Impl;
    Impl* pimpl;

  public:
    /**
     * @brief Ctor
     */
    MainLoop();

    /**
     * @brief Dtor
     */
    ~MainLoop();

    /* -- avoid copying */
    MainLoop(
        const MainLoop&) = delete;
    MainLoop& operator = (
        const MainLoop&) = delete;

    /**
     * @brief Run the main loop
     *
     * @param runner_ The test runner
     * @return Result of the test
     */
    bool runTest(
        ::OTest2::Runner* runner_);

    /**
     * @brief Do something useful
     */
    void doSomethingUseful(
        const std::string& message_);
};

} /* -- namespace Examples */

} /* -- namespace OTest2 */

#endif /* -- OTest2__EXAMPLES_USERDATA_MAINLOOP_H_ */
