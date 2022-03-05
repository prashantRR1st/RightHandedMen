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

export default class WorkoutChart extends Component{
    // time-force data
    data = [
        {time: 1, force: 11,},
        {time: 2, force: 15,},
        {time: 3, force: 5,},
        {time: 4, force: 10,},
        {time: 5, force: 9,},
        {time: 6,force: 10,},
    ];

    render(){
        return(
            <h1 className="text-heading">Workout Summary
                {/* graph code */}
                <ResponsiveContainer width="100%" aspect={3}>
                    <LineChart data={this.data} margin={{ right: 300 }}>
                        <CartesianGrid />
                            <XAxis dataKey="time (s)" interval={'preserveStartEnd'} />
                        <YAxis></YAxis>
                        <Legend />
                        <Tooltip />
                        <Line dataKey="force"
                            stroke="black" activeDot={{ r: 8 }} />
                    </LineChart>
                </ResponsiveContainer>
            </h1>
        );
    }
};