scriptpath=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

ngx_version=1.19.10
ngx_image=${ngx_version}-alpine

ngx_dir=/usr/src/nginx-${ngx_version}

docker-dev-headers-image: Dockerfile.headers
	@docker build  -f Dockerfile.headers -t nginx-headers .

setup-dev-headers: docker-dev-headers-image
	@docker run nginx-headers tar -c ${ngx_dir}/src ${ngx_dir}/objs | tar x -C ${scriptpath}/dev --strip-components 3 --wildcards "*.h"

docker-image:
	@docker build -t foo-nginx .

docker-run:
	@docker run --rm --name foo-nginx-container -p 8000:8000 -v ${scriptpath}/dev/nginx:/etc/nginx:ro foo-nginx

clear:
	@rm -r $(keys_path)
