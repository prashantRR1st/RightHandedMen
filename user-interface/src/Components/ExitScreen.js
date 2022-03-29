import React, { useState, useEffect } from "react";
import Header from "./Header";
import { Grid } from '@mui/material';
import SummaryTable from "./SummaryTable";
import SummaryChart from "./SummaryChart";
import Typography from '@mui/material/Typography';
import results from "../generated_input.json"

export default function ExitScreen() {
    let [data, setData] = useState();
    let [details, setDetails] = useState({
        duration: '',
        cycles: 0,
        scale: 1,
        accuracy: ''
    });

    function msToTime(duration) {
        var milliseconds = Math.floor(duration % 1000),
          seconds = Math.floor((duration / 1000) % 60),
          minutes = Math.floor((duration / (1000 * 60)) % 60),
          hours = Math.floor((duration / (1000 * 60 * 60)) % 24);
      
        hours = (hours < 10) ? "0" + hours : hours;
        minutes = (minutes < 10) ? "0" + minutes : minutes;
        seconds = (seconds < 10) ? "0" + seconds : seconds;
      
        return hours + ":" + minutes + ":" + seconds + "." + milliseconds;
    }
    useEffect(() => {
        // declare the data fetching function
        const fetchData = async () => {
            const res = await fetch('http://127.0.0.1:8887/tracking_output.json', {mode:'cors'});
            const json = await res.json();
            let results_len = results['buffer'].length;
            let displayData = [];
            let errorSum = 0;
            let scale = 0.15;
        
            json.forEach((element, index) => {
                let profile_index = results_len > index? index : index%results_len;
                let profileValue = results['buffer'][profile_index];
                displayData.push(
                    {
                        time: index,
                        live_force: element,
                        profile: profileValue
                    }
                );
                
                errorSum = errorSum + (((profileValue*scale)-element)**2);
                console.log(errorSum);
            })
            setData(displayData);
            let cycles = displayData.length/results_len;
            setDetails({
                duration: msToTime((results['totalTime']*cycles)),
                cycles: cycles.toFixed(2),
                scale: scale,
                error: ((100*(Math.sqrt(errorSum)/displayData.length)).toFixed(2).toString() + '%')
            });
        }
        fetchData();
    }, [])
    return(
        <div style={{backgroundColor: '#121212', height: '100%'}}>
            <Header></Header>
            <Typography gutterBottom variant="h4" component="div" style={{textAlign: 'center', color: '#ffffff'}}>
                Workout Summary
            </Typography>
            <Grid 
                container
                direction="row"
                alignItems="center"
                justifyContent="center"
                style={{padding: '1%'}}
            >
                <Grid item xs={8} style={{padding: '1%'}}>
                <SummaryChart data={data}></SummaryChart>
                </Grid>
                <Grid item xs={4} style={{padding: '1%'}}>
                    <SummaryTable details={details}></SummaryTable>
                </Grid>
            </Grid>
            
        </div>
    );
};
