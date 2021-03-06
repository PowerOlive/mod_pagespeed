/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef NET_INSTAWEB_REWRITER_PUBLIC_CSS_INLINE_IMPORT_TO_LINK_FILTER_H_
#define NET_INSTAWEB_REWRITER_PUBLIC_CSS_INLINE_IMPORT_TO_LINK_FILTER_H_

#include "pagespeed/kernel/base/basictypes.h"
#include "pagespeed/kernel/html/empty_html_filter.h"

namespace net_instaweb {

class HtmlCharactersNode;
class HtmlElement;
class RewriteDriver;
class Statistics;
class Variable;

// Filter to rewrite style tags of the form:
//   <style type="text/css" ...>
//     @import url(URL1);
//     @import url(URL2);
//     foobar
//   </style>
// to
//   <link type="text/css" ... rel="stylesheet" href="URL1"/>
//   <link type="text/css" ... rel="stylesheet" href="URL2"/>
//   <style type="text/css" ...>
//     foobar
//   </style>
class CssInlineImportToLinkFilter : public EmptyHtmlFilter {
 public:
  explicit CssInlineImportToLinkFilter(RewriteDriver* driver,
                                       Statistics* statistics);
  ~CssInlineImportToLinkFilter() override;

  static void InitStats(Statistics* statistics);

  void StartDocument() override;
  void EndDocument() override;

  void StartElement(HtmlElement* element) override;
  void EndElement(HtmlElement* element) override;

  void Flush() override;

  // HTML Events we expect to be in a <style> element.
  void Characters(HtmlCharactersNode* characters) override;

  const char* Name() const override { return "InlineImportToLinkCss"; }

 private:
  void ResetState();
  void InlineImportToLinkStyle();

  RewriteDriver* driver_;
  // The style element we are in (if it hasn't been flushed).
  // If we are not in a style element, style_element_ == NULL.
  HtmlElement* style_element_;
  // The characters inside the style element we are in.
  HtmlCharactersNode* style_characters_;
  // Statistics count of the number of times we rewrite a style element.
  Variable* counter_;

  DISALLOW_COPY_AND_ASSIGN(CssInlineImportToLinkFilter);
};

}  // namespace net_instaweb

#endif  // NET_INSTAWEB_REWRITER_PUBLIC_CSS_INLINE_IMPORT_TO_LINK_FILTER_H_
