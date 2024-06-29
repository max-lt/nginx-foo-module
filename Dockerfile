FROM nginx:1.27.0-alpine3.19-slim as base

FROM base as builder

ARG FOO_MODULE_PATH=/usr/local/lib/ngx-foo-module

RUN apk add --no-cache --virtual .build-deps \
  # nginx
  gcc \
  libc-dev \
  pcre-dev \
  zlib-dev \
  linux-headers \
  make \
  curl

# Download and extract nginx source code
RUN mkdir -p /usr/src \
  && curl -fSL http://nginx.org/download/nginx-${NGINX_VERSION}.tar.gz | tar -zxC /usr/src

ADD config $FOO_MODULE_PATH/config
ADD src $FOO_MODULE_PATH/src

RUN cd /usr/src/nginx-${NGINX_VERSION} \
  && ./configure --with-compat --add-dynamic-module=$FOO_MODULE_PATH \
  && make modules

# Add module to base image
FROM base as foo-nginx

COPY --from=builder /usr/src/nginx-${NGINX_VERSION}/objs/ngx_http_foo_module.so /usr/lib/nginx/modules/ngx_http_foo_module.so

RUN sed -i '1iload_module modules/ngx_http_foo_module.so;' /etc/nginx/nginx.conf
