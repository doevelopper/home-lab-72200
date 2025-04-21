ARG DEBIAN_FRONTEND=noninteractive

# This is the service stage for downloading
# Output is stored in /srv/downloads in accordance with FHS
FROM alpine/git AS downloads_srv
RUN git clone --depth 1 https://github.com/microsoft/vcpkg.git /srv/downloads/vcpkg
RUN git clone --branch release --depth 1 https://github.com/ninja-build/ninja.git /srv/downloads/ninja

# Base stage with non-vital for development utils common for vcpkg_srv and dev
FROM ubuntu:24.04 AS base
ARG DEBIAN_FRONTEND
# Delete default user
RUN touch /var/mail/ubuntu && \
chown ubuntu /var/mail/ubuntu && \
userdel -r ubuntu
# Install basic utilities
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
	--mount=target=/var/cache/apt,type=cache,sharing=locked \
	apt-get update && \
	apt-get install -y --no-install-recommends \
		curl \
		tar \
		unzip \
		zip \
		software-properties-common \
		ca-certificates

FROM base AS cmake
ARG DEBIAN_FRONTEND
# Setup latest git and some necessary utility
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
	--mount=target=/var/cache/apt,type=cache,sharing=locked \
	add-apt-repository ppa:git-core/ppa && \
	apt-get install -y --no-install-recommends \
		git \
		wget \
		gpg \
		lsb-release
# Setup kitware repository for apt
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
	--mount=target=/var/cache/apt,type=cache,sharing=locked \
	KEY=/usr/share/keyrings/kitware-archive-keyring.gpg && \
	wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null |\
		gpg --dearmor - |\
		tee $KEY >/dev/null && \
	echo "deb [signed-by=$KEY] https://apt.kitware.com/ubuntu $(lsb_release -cs) main" |\
		tee /etc/apt/sources.list.d/kitware.list > /dev/null && \
	apt-get update && \
	rm $KEY && \
	apt-get install -y kitware-archive-keyring
# Use updated apt lists to install most recent versions of tools
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
	--mount=target=/var/cache/apt,type=cache,sharing=locked \
	apt-get install -y \
		python3-setuptools \
		pkg-config \
		clang \
		build-essential \
		gdb \
		cmake \
		clang-format \
		ssh-client

FROM cmake AS ninja_srv
COPY --link --from=downloads_srv /srv/downloads/ninja /usr/local/src/ninja
RUN --mount=type=tmpfs,target=/tmp/bld_nja_from_src \
	mv /usr/local/src/ninja /tmp/bld_nja_from_src && \
	cd /tmp/bld_nja_from_src/ninja && \
	cmake -B build -DCMAKE_BUILD_TYPE:STRING=Release -DBUILD_TESTING:BOOL=OFF && \
	cmake --build build --target install --parallel && \
	ln -s /usr/local/bin/ninja /usr/local/bin/ninja-build
RUN mkdir -p /srv/ninja && cp -r /usr/local/bin/ninja /srv/ninja/ninja
ENV PATH="/usr/local/bin:$PATH"

# Service stage for bootstrapping vcpkg and populating cache
# VCPKG is installed system wide in accordance with FHS
# Output is stored in /srv/vcpkg in accordance with FHS
FROM cmake AS vcpkg_srv
ARG DEBIAN_FRONTEND
ENV VCPKG_SRV_INSTALLED_DIRNAME=/var/opt/vcpkg \
	VCPKG_SRV_CACHE_DIRNAME=/var/cache/vcpkg
ENV VCPKG_ROOT=/opt/vcpkg \
	VCPKG_DEFAULT_BINARY_CACHE=${VCPKG_SRV_CACHE_DIRNAME}/binaries \
	VCPKG_DOWNLOADS=${VCPKG_SRV_CACHE_DIRNAME}/downloads
# Install vcpkg
COPY --link --from=downloads_srv /srv/downloads/vcpkg ${VCPKG_ROOT}
RUN mkdir -p \
		/opt/bin \
		/srv/vcpkg && \
	ln -sf ${VCPKG_DOWNLOADS} /srv/vcpkg/downloads && \
	ln -sf ${VCPKG_DEFAULT_BINARY_CACHE} /srv/vcpkg/binary_cache && \
	ln -sf ${VCPKG_SRV_INSTALLED_DIRNAME} /srv/vcpkg/results && \
	ln -sf ${VCPKG_ROOT} /srv/vcpkg/vcpkg_root && \
	ln -sf /usr/local/bin/ninja /usr/local/bin/ninja-build
COPY --link scripts/container/redirect_to_vcpkg.sh /opt/bin/vcpkg
COPY --link --from=ninja_srv /srv/ninja/ninja /usr/local/bin/ninja
ENV PATH="/opt/bin:/usr/local/bin:$PATH"
RUN --mount=type=cache,target=${VCPKG_DOWNLOADS} \
	--mount=type=cache,target=${VCPKG_DEFAULT_BINARY_CACHE} \
	${VCPKG_ROOT}/bootstrap-vcpkg.sh -disableMetrics -skipDependencyChecks
# Install packages, according to manifest.
# The results are then output to /srv/vcpkg (including the vcpkg itself)
# Building happens in tmpfs to possibly speedup the process
COPY --link vcpkg.json ${VCPKG_SRV_INSTALLED_DIRNAME}/
RUN --mount=type=cache,target=${VCPKG_DOWNLOADS} \
	--mount=type=cache,target=${VCPKG_DEFAULT_BINARY_CACHE} \
	--mount=type=tmpfs,target=/tmp/vcpkg_inst/ \
	mv ${VCPKG_SRV_INSTALLED_DIRNAME}/vcpkg.json /tmp/vcpkg_inst/ && \
	cd /tmp/vcpkg_inst && \
	(vcpkg install --keep-going || true) && \
	cp -r ${VCPKG_SRV_CACHE_DIRNAME} /var/tmp/vcpkg
# cp out then mv in to take advantage of BOTH caching and parallel docker build
RUN rm -rf ${VCPKG_SRV_CACHE_DIRNAME} && mv /var/tmp/vcpkg ${VCPKG_SRV_CACHE_DIRNAME}

FROM cmake AS ninja
COPY --link --from=ninja_srv /srv/ninja/ninja /usr/local/bin/ninja
RUN ln -s /usr/local/bin/ninja /usr/local/bin/ninja-build
ENV PATH="/usr/local/bin/:$PATH"

FROM ninja AS root
ARG HOME=/root
ARG HLOCAL=${HOME}/.local HCACHE=${HOME}/.cache
ENV VCPKG_ROOT=${HLOCAL}/vcpkg
# Setup user
RUN echo "root:root" | chpasswd
# Finally
USER root
RUN mkdir -p ${HOME}/workspace
WORKDIR ${HOME}/workspace
# Setup vcpkg
COPY --link --from=vcpkg_srv /srv/vcpkg/vcpkg_root ${VCPKG_ROOT}
COPY --link --from=vcpkg_srv /srv/vcpkg/downloads ${VCPKG_ROOT}/downloads
COPY --link --from=vcpkg_srv /srv/vcpkg/binary_cache ${HCACHE}/vcpkg/archives
RUN mkdir -p ${HLOCAL}/bin && ln -s ${VCPKG_ROOT}/vcpkg ${HLOCAL}/bin

FROM ninja AS non-root
ARG DEBIAN_FRONTEND
ARG USER_ID
ARG USER_NAME
ARG USER_GID
ARG USER_GROUP
ARG HOME=/home/${USER_NAME}
# Setup user
RUN --mount=target=/var/lib/apt/lists,type=cache,sharing=locked \
	--mount=target=/var/cache/apt,type=cache,sharing=locked \
	apt-get install -y \
		sudo
RUN groupadd -g ${USER_GID} ${USER_GROUP} && \
	useradd ${USER_NAME} -u ${USER_ID} -g ${USER_GROUP} -G sudo -m -s /bin/bash && \
	echo "${USER_NAME}:${USER_NAME}" | chpasswd
# Finally
USER ${USER_NAME}
RUN mkdir ${HOME}/workspace
WORKDIR ${HOME}/workspace
# Setup vcpkg
ARG HLOCAL=${HOME}/.local HCACHE=${HOME}/.cache
ENV VCPKG_ROOT=${HLOCAL}/vcpkg
COPY --link --from=vcpkg_srv --chown=${USER_ID}:${USER_GID} \
	/srv/vcpkg/vcpkg_root \
	${VCPKG_ROOT}
COPY --link --from=vcpkg_srv --chown=${USER_ID}:${USER_GID} \
	/srv/vcpkg/downloads \
	${VCPKG_ROOT}/downloads
COPY --link --from=vcpkg_srv --chown=${USER_ID}:${USER_GID} \
	/srv/vcpkg/binary_cache \
	${HCACHE}/vcpkg/archives
RUN mkdir -p ${HLOCAL}/bin && ln -s ${VCPKG_ROOT}/vcpkg ${HLOCAL}/bin

