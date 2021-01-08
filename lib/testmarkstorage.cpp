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

#include <testmarkstorage.h>

#include <algorithm>
#include <assert.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <strstream>

#include <base64istream.h>
#include <base64ostream.h>
#include <bzip2istream.h>
#include <bzip2ostream.h>
#include <exctestmarkin.h>
#include <testmark.h>
#include <testmarkfactory.h>
#include <testmarkinbinios.h>
#include <testmarkoutbinios.h>
#include <testmarkptr.h>
#include <utils.h>

namespace OTest2 {

namespace {

bool isLineEmpty(
    const std::string& line_) {
  return std::all_of(
      line_.begin(), line_.end(), [](char c_) { return std::isspace(c_); });
}

std::string::size_type searchForSeparator(
    const std::string& line_) {
  bool escaped_(false);
  const std::string::size_type length_(line_.size());
  for(std::string::size_type i_(0); i_ < length_; ++i_) {
    char c_(line_[i_]);
    if(escaped_) {
      escaped_ = false;
    }
    else if(c_ == '\\') {
      escaped_ = true;
    }
    else if(c_ == ':') {
      return i_;
    }
  }
  return std::string::npos;
}

std::string escapeKey(
    const std::string& unescaped_key_) {
  std::ostringstream oss_;
  for(char c_ : unescaped_key_) {
    if(c_ == ':' || c_ == '\\' || std::isspace(c_))
      oss_.put('\\');
    oss_.put(c_);
  }
  return oss_.str();
}

std::string unescapeKey(
    const std::string& escaped_key_) {
  std::ostringstream oss_;
  bool escape_(false);
  for(char c_ : escaped_key_) {
    if(escape_) {
      oss_.put(c_);
      escape_ = false;
    }
    else {
      if(c_ == '\\') {
        escape_ = true;
      }
      else {
        oss_.put(c_);
      }
    }
  }
  return oss_.str();
}

} /* -- namespace */

struct TestMarkStorage::Impl {
    TestMarkFactory* factory;
    std::string storage_file;

    typedef std::map<std::string, TestMarkPtr> Storage;
    Storage storage;
    bool changed;

    /* -- avoid copying */
    Impl(
        const Impl&) = delete;
    Impl& operator = (
        const Impl&) = delete;

    explicit Impl(
        TestMarkFactory* factory_,
        const std::string& storage_file_);
    ~Impl();
};

TestMarkStorage::Impl::Impl(
    TestMarkFactory* factory_,
    const std::string& storage_file_) :
  factory(factory_),
  storage_file(storage_file_),
  storage(),
  changed(false) {
  assert(factory != nullptr);

  /* -- read the test marks from the file */
  std::ifstream ifs_(storage_file.c_str());
  while(!!ifs_) {
    std::ostringstream oss_;
    ifs_.get(*oss_.rdbuf());  /* -- get the whole line */
    ifs_.get();               /* -- skip the newline */
    std::string line_(oss_.str());

    /* -- ignore empty lines */
    if(isLineEmpty(line_))
      continue;

    /* -- parse the key */
    auto sep_index_(searchForSeparator(line_));
    if(sep_index_ <= 0 || sep_index_ == std::string::npos) /* -- wrong line */
      throw ExcTestMarkIn("invalid format of the test mark storage file " + storage_file);
    std::string key_(line_.data(), line_.data() + sep_index_);

    /* -- parse the test mark */
    std::istrstream iss_(
        line_.data() + sep_index_ + 1, line_.size() - sep_index_ - 1);
    Base64IStream base64i_(&iss_);
    Bzip2IStream bzip2i_(&base64i_);
    TestMarkInBinIOS reader_(&bzip2i_);
    TestMarkPtr testmark_(TestMarkInBinIOS::deserialize(*factory, reader_));

    /* -- insert into the map */
    storage.insert({unescapeKey(key_), testmark_});
  }
}

TestMarkStorage::Impl::~Impl() {
  /* -- store the storage if it changed */
  if(changed) {
    std::ofstream ofs_(storage_file.c_str());
    for(const auto& mark_ : storage) {
      /* -- write key */
      ofs_ << escapeKey(mark_.first) << ':';

      /* -- write the mark */
      {
        Base64OStream base64o_(&ofs_);
        Bzip2OStream bzip2o_(&base64o_);
        TestMarkOutBinIOS writer_(&bzip2o_);
        mark_.second->serializeMark(writer_);
      }

      /* -- record separator */
      ofs_ << '\n';
    }
  }
}

TestMarkStorage::TestMarkStorage(
    TestMarkFactory* factory_,
    const std::string& storage_file_) :
  pimpl(new Impl(factory_, storage_file_)) {

}

TestMarkStorage::~TestMarkStorage() {
  odelete(pimpl);
}

void TestMarkStorage::setTestMark(
    const std::string& key_,
    TestMarkPtr test_mark_) {
  assert(!key_.empty() && test_mark_ != nullptr);

  pimpl->storage[key_] = test_mark_;
  pimpl->changed = true;
}

TestMarkPtr TestMarkStorage::getTestMark(
    const std::string& key_) const {
  assert(!key_.empty());

  auto iter_(pimpl->storage.find(key_));
  if(iter_ != pimpl->storage.end())
    return (*iter_).second;
  else
    return TestMarkPtr();
}

} /* -- namespace OTest2 */
