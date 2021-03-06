FROM nginx:1.19.10-alpine as base

# Download and extract nginx source code
FROM base as prepare

RUN apk add --no-cache \
  # nginx
  gcc \
  libc-dev \
  make \
  openssl-dev \
  pcre-dev \
  zlib-dev \
  linux-headers \
  curl \
  gnupg \
  libxslt-dev \
  gd-dev

RUN curl -fSL http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz -o nginx.tar.gz \
  && mkdir -p /usr/src \
  && tar -zxC /usr/src -f nginx.tar.gz \
  && rm nginx.tar.gz

# Compile module
FROM prepare as builder

ARG FOO_MODULE_PATH=/usr/local/lib/ngx-foo-module

RUN mkdir -p $FOO_MODULE_PATH/src

ADD config $FOO_MODULE_PATH/config
ADD src $FOO_MODULE_PATH/src

RUN cd /usr/src/nginx-${NGINX_VERSION} \
  && ./configure --with-compat --add-dynamic-module=$FOO_MODULE_PATH \
  && make modules

# Add module to base image
FROM base

COPY --from=builder /usr/src/nginx-${NGINX_VERSION}/objs/ngx_http_foo_module.so /usr/lib/nginx/modules/ngx_http_foo_module.so

RUN sed -i '1iload_module modules/ngx_http_foo_module.so;' /etc/nginx/nginx.conf
