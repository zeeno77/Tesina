import '../node_modules/bootstrap/dist/css/bootstrap.min.css';
import React, { Component } from 'react';
import { render } from 'react-dom';
import {RadialGauge} from 'react-canvas-gauges';
import './style.css';
import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';

class App extends Component {
  constructor() {
    super();
    this.state = {
      name: 'React'
    };
  }

  componentDidMount() {
    
    let currentTemp = 0, currentHumidity = 0, json= "", auxTemp= -999,auxHumidity= -999;

    this.interval = setInterval(()=>{
        
        fetch("http://localhost:8000/muestra/sensor/Temperatura").then(response => 
          response.json().then(data => ({
              data: data,
              status: response.status
          })
      ).then(res => {
          auxTemp = parseFloat(res.data.valor)
          if(auxTemp != -999){
            currentTemp = auxTemp
          }
          json = res.data
          console.log(res.status, res.data)
      }));
      
      fetch("http://localhost:8000/muestra/sensor/Humedad").then(response => 
      response.json().then(data => ({
          data: data,
          status: response.status
      })
      ).then(res => {
        auxHumidity = parseFloat(res.data.valor)
      if(auxHumidity != -999){
        currentHumidity = auxHumidity
      }
      json = res.data
      console.log(res.status, res.data)
    }));

        
        console.log("valor: " )
        console.log(currentTemp)
        console.log("json: " )
        console.log(json)

        this.setState({
          temperature : currentTemp,
          humidity : currentHumidity
        })

    }, 5000);
    
  
  }

  componentWillUnmount() {
    if (this.interval) {
      clearInterval(this.interval)
    }
  }
  
  render() {
    return (
      <Container>
      <Row className="justify-content-md-center">
        <Col md={6}>
        <RadialGauge
           data-width='300'
           data-height='300'
           units='°C'
           title='Temperatura'
           value={this.state.temperature}
           minValue={0}
           maxValue={45}
           majorTicks={['0', '5', '15', '20', '25', '30', '35', '40', '45']}
           minorTicks={1}
        ></RadialGauge>
        </Col>
        <Col md={6}><RadialGauge
         data-width='300'
         data-height='300'
         units='%'
         title='Humedad'
         value={this.state.humidity}
         minValue={0}
         maxValue={100}
         majorTicks={['0', '10', '20', '30', '40', '50', '60', '70', '80','90','100']}
         minorTicks={1}
      ></RadialGauge>
      </Col>
      </Row>
    </Container>
    );
  }
}

render(<App />, document.getElementById('root'));
