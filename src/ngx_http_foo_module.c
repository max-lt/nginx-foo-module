#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
  ngx_flag_t foo;
} ngx_http_foo_loc_conf_t;

static ngx_int_t ngx_http_foo_handler(ngx_http_request_t *r);

// Configuration functions
static ngx_int_t ngx_http_foo_init(ngx_conf_t *cf);
static void * ngx_http_foo_create_conf(ngx_conf_t *cf);
static char * ngx_http_foo_merge_conf(ngx_conf_t *cf, void *parent, void *child);

static ngx_command_t ngx_http_foo_commands[] = {
  // Syntax: foo on | off; Default: foo off;
  { ngx_string("foo"),
    NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
    ngx_conf_set_flag_slot,
    NGX_HTTP_LOC_CONF_OFFSET,
    offsetof(ngx_http_foo_loc_conf_t, foo),
    NULL },

  ngx_null_command
};


static ngx_http_module_t ngx_http_foo_module_ctx = {
  NULL,                          /* preconfiguration */
  ngx_http_foo_init,             /* postconfiguration */

  NULL,                          /* create main configuration */
  NULL,                          /* init main configuration */

  NULL,                          /* create server configuration */
  NULL,                          /* merge server configuration */

  ngx_http_foo_create_conf,             /* create location configuration */
  ngx_http_foo_merge_conf               /* merge location configuration */
};


ngx_module_t ngx_http_foo_module = {
  NGX_MODULE_V1,
  &ngx_http_foo_module_ctx,     /* module context */
  ngx_http_foo_commands,        /* module directives */
  NGX_HTTP_MODULE,                     /* module type */
  NULL,                                /* init master */
  NULL,                                /* init module */
  NULL,                                /* init process */
  NULL,                                /* init thread */
  NULL,                                /* exit thread */
  NULL,                                /* exit process */
  NULL,                                /* exit master */
  NGX_MODULE_V1_PADDING
};


// https://nginx.org/en/docs/dev/development_guide.html#http_response_body
static ngx_int_t ngx_http_foo_handler(ngx_http_request_t *r)
{
    const ngx_http_foo_loc_conf_t *conf = ngx_http_get_module_loc_conf(r, ngx_http_foo_module);

    // Decline if foo is off
    if (conf->foo < 1)
    {
      return NGX_DECLINED;
    }

    ngx_int_t     rc;
    ngx_buf_t    *b;
    ngx_chain_t   out;
    ngx_str_t     message = ngx_string("Hello Foo!");

    /* send header */

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = message.len;

    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    /* send body */

    b = ngx_calloc_buf(r->pool);
    if (b == NULL) {
        return NGX_ERROR;
    }

    b->last_buf = (r == r->main) ? 1 : 0;
    b->last_in_chain = 1;

    b->memory = 1;

    b->pos = message.data;
    b->last = b->pos + message.len;

    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out);
}


static ngx_int_t ngx_http_foo_init(ngx_conf_t *cf)
{
  ngx_http_handler_pt        *h;
  ngx_http_core_main_conf_t  *cmcf;

  cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

  h = ngx_array_push(&cmcf->phases[NGX_HTTP_CONTENT_PHASE].handlers);
  if (h == NULL)
  {
    return NGX_ERROR;
  }

  *h = ngx_http_foo_handler;

  return NGX_OK;
}


static void * ngx_http_foo_create_conf(ngx_conf_t *cf)
{
  ngx_http_foo_loc_conf_t *conf;

  conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_foo_loc_conf_t));
  if (conf == NULL)
  {
    ngx_conf_log_error(NGX_LOG_WARN, cf, 0, "foo create conf: conf==NULL");
    return NULL;
  }

  // Initialize variables
  conf->foo = NGX_CONF_UNSET;

  return conf;
}


static char * ngx_http_foo_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
  ngx_http_foo_loc_conf_t *prev = parent;
  ngx_http_foo_loc_conf_t *conf = child;

  ngx_conf_merge_value(conf->foo, prev->foo, 0);

  return NGX_CONF_OK;
}
