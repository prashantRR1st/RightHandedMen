import { Component } from "react";
import {
    LineChart,
    ResponsiveContainer,
    Legend, Tooltip,
    Line,
    XAxis,
    YAxis,
    CartesianGrid
} from 'recharts';
import results from "../generated_input.json"

function  getResultsData() {
    let displayData = [];

    results['buffer'].forEach((element, index) => {
        displayData.push(
            {
                time: index,
                force: element
            }
        )
    })
    console.log(displayData);
    return(displayData);
}

export default function CalibrationChart() {    
    let data = getResultsData();

    return(
        <div>
            <ResponsiveContainer width="100%" aspect={3}>
                <LineChart data={data} margin={{ right: 300 }}>
                    <CartesianGrid />
                        <XAxis dataKey="time (s)" interval={'preserveStartEnd'} />
                    <YAxis></YAxis>
                    <Legend />
                    <Tooltip />
                    <Line dataKey="force"
                        stroke="black" activeDot={{ r: 8 }} />
                </LineChart>
            </ResponsiveContainer>
            
        </div> 
    );

};