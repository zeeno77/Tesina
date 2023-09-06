from python:3.9.1
RUN mkdir /app
WORKDIR /app
RUN cd /app
COPY fastapi /app
RUN pip install -r requirements.txt
EXPOSE 8000
RUN rm -rf /app
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000", "--log-level", "debug"]
