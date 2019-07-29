FROM quay.io/fenicsproject/dolfinx:latest

########################################################
# Essential packages for remote debugging and login in
########################################################

# RUN apt-get update && apt-get upgrade -y && apt-get install -y \
#     apt-utils gcc g++ openssh-server cmake build-essential gdb gdbserver rsync vim
# 
# ADD . /root
# WORKDIR /root
# 
# # Taken from - https://docs.docker.com/engine/examples/running_ssh_service/#environment-variables
# 
# RUN mkdir /var/run/sshd
# RUN echo 'root:root' | chpasswd
# RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config
# 
# # SSH login fix. Otherwise user is kicked off after login
# RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
# 
# ENV NOTVISIBLE "in users profile"
# RUN echo "export VISIBLE=now" >> /etc/profile
# 
# # 22 for ssh server. 7777 for gdb server.
# EXPOSE 22 7777
# 
# RUN useradd -ms /bin/bash debugger
# RUN echo 'debugger:pwd' | chpasswd

########################################################
# Add custom packages and development environment here
########################################################
# docker build -t mvc-xdmf .
# Clone this repository
RUN git clone -b iitrabhi/mvc-xdmf https://github.com/iitrabhi/dolfinx.git

# Build and install dolfinx
RUN cd dolfinx/cpp && \
	mkdir build && \
	cd build && \
	cmake  .. && \
	make install -j 2

# Install dolfinx python package
RUN pip3 install -e /root/dolfinx/python/.

# Install meshio python package
RUN git clone -b iitrabhi/mvc-xdmf https://github.com/iitrabhi/meshio.git
RUN pip3 install -e /root/meshio/.

########################################################

CMD ["/usr/sbin/sshd", "-D"]
