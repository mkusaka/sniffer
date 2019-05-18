FROM centos:centos7

RUN yum install -y gcc gcc-c++ make log4cpp boost boost-devel epel-release git && yum -y --enablerepo=epel install log4cpp log4cpp-devel libcurl libcurl-devel && yum clean all

WORKDIR /home/src/
