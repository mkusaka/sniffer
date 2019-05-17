FROM centos:centos7

RUN yum install -y gcc gcc-c++ make log4cpp boost boost-devel epel-release && yum -y --enablerepo=epel install log4cpp log4cpp-devel

WORKDIR /home/src/

# RUN curl -LO https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.gz && tar xfvz boost_1_68_0.tar.gz &&
