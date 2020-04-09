---
title: Examples
excerpt: "Collection of examples how to use the OTest2 framework."
permalink: /examples/
---

{% assign entries = site.examples | sort: 'lesson' %}
{% for example in entries %}
## [{{ example.title }}]({{ example.url | relative_url }})

{{ example.excerpt }}

{% endfor %}