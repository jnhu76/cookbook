<template>
  <h1>{{ msg }}</h1>

    <div id="JMS_main" class="main">
        <table id="landmine"></table>
        <div id="operation">
            <div class="tip">剩余雷数：<span class="light red" id="landMineCount">0</span> 个</div>
            <div class="tip">持续时间: <span class="light f60" id="costTime">0</span> 秒</div>
            <fieldset>
                <legend>难度选择：</legend>
                <input type="radio" name="level" id="llevel" checked="checked" value="10" /><label for="llevel">初级（10*10）</label><br />
                <input type="radio" name="level" id="mlevel" value="15" /><label for="mlevel">中级（15*15）</label><br />
                <input type="radio" name="level" id="hlevel" value="20" /><label for="hlevel">高级（20*20）</label><br />
            </fieldset>
            <input type="button" id="begin" value="开始游戏" /><br />
            <div class="tip txtleft">提示：
                <ul>
                    <li>1、点击“开始游戏”游戏开始计时</li>
                    <li>2、游戏过程中点击“开始游戏”将开始新游戏</li>
                </ul>
            </div>
        </div>
    </div>
</template>

<script lang="ts">
import { ref, defineComponent } from 'vue'
export default defineComponent({
  name: 'minesweeper',
  props: {
    msg: {
      type: String,
      required: true
    }
  },
  setup: () => {
    const count = ref(0)
    return { count }
  }
})
</script>

<style scoped>
.main {
  margin: 10px auto;
  padding: 20px;
  background: #eee;
  width: 600px;
  zoom: 1;
}
.main table {
  background: #ccc;
  float: left;
}

.main table td {
  border: 2px outset #eee;
  font-size: 20px;
  width: 32px;
  height: 32px;
  text-align: center;
  cursor: pointer;
}

.main table td:hover {
  background-color: #aaa;
}
.main #operation {
  width: 180px;
  float: right;
  text-align: center;
}

.landMine {
  background-image: url(mine.png);
  background-position: center;
  background-repeat: no-repeat;
}

.main table td.normal {
  border: 2px solid #eee;
  background-color: #aaa;
}

.main table td.normal:hover {
  background-color: #aaa;
}

.flag {
  background-image: url(flag.png);
  background-position: center;
  background-repeat: no-repeat;
}

.main:after {
  clear: both;
  display: block;
  content: '';
  line-height: 0;
  height: 0;
  visibility: hidden;
}
.main .tip {
  font-size: 14px;
  margin: 5px;
}

.main .tip ul {
}

.main .tip ul li {
  margin: 5px 0;
  line-height: 20px;
}
.main .light {
  font-size: 30px;
}
.main .red {
  color: red;
}
.main .f60 {
  color: #f60;
}
.main input[type='button'] {
  padding: 2px 10px;
  margin: 5px;
  font-size: 20px;
  cursor: pointer;
}
.main .txtleft {
  text-align: left;
}

.main input[type='radio'],
.main fieldset label {
  cursor: pointer;
}

.main fieldset {
  margin: 10px 0;
  line-height: 25px;
}
</style>
