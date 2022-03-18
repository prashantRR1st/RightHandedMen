import './App.css';
import { Link } from "react-router-dom";
import StartScreen from './Components/StartScreen';

function App(){
  return(
    <div className="App" style={{backgroundColor: '#121212', height: '100%'}}>
      <StartScreen></StartScreen>
    </div>
  );
}

export default App;