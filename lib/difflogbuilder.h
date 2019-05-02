/*
 * Copyright (C) 2019 Ondrej Starek
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
#ifndef OTest2__LIB_DIFFLOGBUILDER_H_
#define OTest2__LIB_DIFFLOGBUILDER_H_

namespace OTest2 {

/**
 * @brief Generic interface of the log builder
 *
 * Log builders construct a log of diff changes which can be processed
 * later.
 */
class DiffLogBuilder {
  public:
    /**
     * @brief Ctor
     */
    DiffLogBuilder();

    /**
     * @brief Dtor
     */
    virtual ~DiffLogBuilder();

    /* -- avoid copying */
    DiffLogBuilder(
        const DiffLogBuilder&) = delete;
    DiffLogBuilder& operator =(
        const DiffLogBuilder&) = delete;

    /**
     * @brief Add change of both sequences
     *
     * @param left_index_ index in the left sequence
     * @param right_index_ index in the right sequence
     */
    virtual void addChange(
        int left_index_,
        int right_index_) = 0;

    /**
     * @brief Add deleted item from the right sequence
     *
     * @param right_index_ index in the right sequence
     */
    virtual void addDelete(
        int right_index_) = 0;

    /**
     * @brief Add inserted item to the left sequence
     *
     * @param left_index_ index in the left sequence
     */
    virtual void addInsert(
        int left_index_) = 0;
};

} /* -- namespace OTest2 */

#endif /* OTest2__LIB_DIFFLOGBUILDER_H_ */
