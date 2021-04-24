scriptpath=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

ngx_version=1.19.10
ngx_image=${ngx_version}-alpine

docker-dev-headers-image:
	@docker build  -f Dockerfile.headers -t nginx-headers .

setup-dev-headers: docker-dev-headers-image
	@docker run nginx-headers tar -c -C /usr/src/nginx-1.19.10/ src | tar x -C ${scriptpath}/dev

docker-image:
	@docker build -t foo-nginx .

docker-run:
	@docker run --rm --name foo-nginx-container -p 8000:8000 -v ${scriptpath}/dev/nginx:/etc/nginx:ro foo-nginx

clear:
	@rm -r $(keys_path)
