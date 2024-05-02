FROM ubuntu

WORKDIR /app

COPY ./bin/release .

CMD ["/app/app"]

EXPOSE 9090