package ypy.client;

import android.content.DialogInterface;
import android.database.Cursor;
import android.media.AudioManager;
import android.media.SoundPool;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.provider.ContactsContract;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.json.JSONArray;
import org.json.JSONObject;

import java.text.SimpleDateFormat;
import java.util.HashMap;
import java.util.TimeZone;

public class MainActivity extends AppCompatActivity {

    private SoundPool sp;
    private HashMap<Integer,Integer> spMap;

    Handler handler1, handler2;

    private ImageView rainImage;
    private TextView currentTime, thresholdTime, red;
    private Button setThreshold;
    private EditText setHour, setMinute, setSecond;

    private long isRain = 0;
    private long threshold = 0;
    private int redSensor = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
        handler1.postDelayed(soundRunnable1, 100);
        handler2.postDelayed(soundRunnable2, 1000);
        setListenser();


    }

    private void init() {
        rainImage = (ImageView)findViewById(R.id.rain_image);
        currentTime = (TextView)findViewById(R.id.time_current);
        thresholdTime = (TextView)findViewById(R.id.time_threadhold);
        red = (TextView)findViewById(R.id.red);
        setThreshold = (Button)findViewById(R.id.set_threshold);

        sp = new SoundPool(2, AudioManager.STREAM_MUSIC,0);
        spMap = new HashMap<Integer,Integer>();
        spMap.put(1, sp.load(this,R.raw.notice, 1));

        handler1 = new Handler();
        handler2 = new Handler();

    }

    private void setListenser() {

        setThreshold.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                LayoutInflater factory = LayoutInflater.from(MainActivity.this);
                View dialogView = factory.inflate(R.layout.dialog_layout, null);
                final AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                builder.setTitle("修改阈值");
                builder.setView(dialogView);
                setHour = (EditText) dialogView.findViewById(R.id.dialogHour);;
                setMinute = (EditText) dialogView.findViewById(R.id.dialogMinute);
                setSecond = (EditText) dialogView.findViewById(R.id.dialogSecond);

                builder.setNegativeButton("放弃修改", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                builder.setPositiveButton("保存修改", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String time1 = setHour.getText().toString()+":"+setMinute.getText().toString()+":" +setSecond.getText().toString();
                        String[] my1 =time1.split(":");
                        int hour1 =Integer.parseInt(my1[0]);
                        int min1 =Integer.parseInt(my1[1]);
                        int sec1 =Integer.parseInt(my1[2]);

                        final int temp =(hour1*3600+min1*60+sec1) *1000;

                        Runnable set_threshold= new Runnable() {
                            @Override
                            public void run() {
                                String baseURL = "http://172.18.71.17:8080/IoT/set/threshold/";
                                String threshold = temp+"";
                                try{
                                    String url = baseURL + threshold;
                                    HttpGet httpGet = new HttpGet(url);
                                    HttpResponse httpResponse = new DefaultHttpClient().execute(httpGet);
                                } catch (Exception e) {
                                    e.printStackTrace();
                                }
                            }
                        };
                        new Thread(set_threshold).start();
                        Toast.makeText(MainActivity.this, "修改成功", Toast.LENGTH_SHORT).show();
                    }
                });
                builder.create();
                builder.show();
            }
        });
    }


    public void playSounds(int sound, int number){
        AudioManager am = (AudioManager)this.getSystemService(this.AUDIO_SERVICE);
        float audioMaxVolumn = am.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
        float audioCurrentVolumn = am.getStreamVolume(AudioManager.STREAM_MUSIC);
        float volumnRatio = audioCurrentVolumn/audioMaxVolumn;
        sp.play(spMap.get(sound), volumnRatio, volumnRatio, 1, number, 1);
    }

    Runnable getTask= new Runnable() { //返回所有电影的信息
        @Override
        public void run() {
            String baseURL = "http://172.18.71.17:8080/IoT/get";
            String result= "";
            String TAG= "get";
            try {

                HttpGet httpGet= new HttpGet(baseURL);
                HttpResponse httpResponse= new DefaultHttpClient().execute(httpGet);
                result = EntityUtils.toString(httpResponse.getEntity(),"UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
            Message msg = new Message();
            Bundle data = new Bundle();
            data.putString("value", result);
            msg.setData(data);
            get_handler.sendMessage(msg);

        }

    };

    Handler get_handler= new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Bundle data = msg.getData();
            String val = data.getString("value");
            String TAG = "json";

            try{
                JSONArray array= new JSONObject(val).getJSONArray("data");
                for (int i = 0; i < array.length(); ++i) {
                    JSONObject temp = array.getJSONObject(i);
                     isRain = Integer.parseInt(temp.get("rain").toString());
                     redSensor = Integer.parseInt(temp.getString("redsensor").toString());
                     threshold = Integer.parseInt(temp.getString("threshold").toString());
                }
                if (isRain > 0) {
                    rainImage.setImageResource(R.mipmap.rain);

                } else {
                    rainImage.setImageResource(R.mipmap.sun);

                }
                if (redSensor > 0) {
                    red.setText("红外预警");
                } else {
                    red.setText("");
                }
                currentTime.setText(timeParse(isRain));
                thresholdTime.setText(timeParse(threshold));


            }
            catch (Exception e) {
                Log.i(TAG, e.toString());
            }
        }
    };

    public  String timeParse(long duration) {
        String time = "" ;
        long hour = duration/3600000;
        long minute = (duration-hour*3600000) / 60000 ;
        long seconds = duration % 60000 ;

        long second = Math.round((float)seconds/1000) ;

        if( hour < 10 ){
            time += "0" ;
        }
        time += hour+":";
        if( minute < 10 ){
            time += "0" ;
        }
        time += minute+":" ;

        if( second < 10 ){
            time += "0" ;
        }
        time += second ;

        return time ;
    }

    private Runnable soundRunnable1 = new Runnable() {
        @Override
        public void run() {
            new Thread(getTask).start();

            handler1.postDelayed(this, 100);
        }
    };

    private Runnable soundRunnable2 = new Runnable() {
        @Override
        public void run() {
            String time1 = currentTime.getText().toString();
            String time2 = thresholdTime.getText().toString();
            String[] my1 =time1.split(":");
            String[] my2 =time2.split(":");
            int hour1 =Integer.parseInt(my1[0]);
            int min1 =Integer.parseInt(my1[1]);
            int sec1 =Integer.parseInt(my1[2]);
            int hour2 =Integer.parseInt(my2[0]);
            int min2 =Integer.parseInt(my2[1]);
            int sec2 =Integer.parseInt(my2[2]);

            isRain =hour1*3600+min1*60+sec1;
            threshold=hour2*3600+min2*60+sec2;

            if (isRain >= threshold ||red.getText().toString().equals("红外预警")) {
                playSounds(1,1);
                if (isRain >= threshold) {
                    currentTime.setTextColor(getResources().getColor(R.color.colorPrimary));
                }
            } else {
                currentTime.setTextColor(getResources().getColor(R.color.grey));
            }

            handler2.postDelayed(this, 1000);
        }
    };


}
